# coding: utf-8
import sys, os, random, copy, socket, time, re, argparse, os
from collections import defaultdict
import numpy as np
import subprocess
import glob
from core.utils.common import recDotDefaultDict, dotDict, recDotDict, separate_path_and_filename, flatten
from pprint import pprint
import matplotlib.pyplot as plt

class FeatureVocab(object):
  def __init__(self, all_tokens):
    counter = Counter(all_tokens)
    self.freq = counter.values
    self.rev_vocab = list(counter.keys())
    self.rev_vocab.insert(0, _UNK)
    self.vocab = OrderedDict([(t, i) for i,t in enumerate(self.rev_vocab)])
    self.size = len(self.vocab)

  def __str__(self):
    return str(self.vocab)

  def token2id(self, token):
    return self.vocab.get(token, self.vocab.get(_UNK))

  def sent2ids(self, sent):
    return [self.token2id(t) for t in sent]

  def paragraph2ids(self, para):
    return [self.sent2ids(s) for s in para]

def read_log(logfile_path):
  logtxt = [l.strip().split() for l in open(logfile_path)]
  if len(logtxt) != 90:
    return
  
  state = []
  candidates = []
  pick = []
  for i in range(int(len(logtxt) / 3)):
    t = i * 3
    _state = [[int(x) for x in logtxt[t]]]
    _candidates = [int(x) for x in logtxt[t+1]]
    _pick = int(logtxt[t+2][0])
    state.append(_state)
    candidates.append(_candidates)
    pick.append(_pick)

  episode = recDotDict({
    'state': np.array(state),
    'candidates': np.array(candidates),
    'pick': np.array(pick),
  })
  return episode

def read_result(resultfile_path):
  result = [int(x) for x in open(resultfile_path).readline().split()]

  if not result[0] in [1, 0] or not result[1] in [1, 0]:
    return None
  return result

def read_logs(evaluation_path, epoch):
  assert os.path.exists(evaluation_path)
  if not epoch:
    _, epoch = separate_path_and_filename(glob.glob(evaluation_path + '/*')[-1])
  epoch = int(epoch)
  evaluation_dir = os.path.join(evaluation_path, '%03d' % epoch) 

  if not os.path.exists(evaluation_dir):
    sys.stderr.write('No such directory: %s\n' % evaluation_dir)
    return 
  
  p1log_paths = [fpath for fpath in glob.glob(evaluation_dir+ '/player1-*')]
  p2log_paths = [fpath for fpath in glob.glob(evaluation_dir+ '/player2-*')]
  result_paths = [fpath for fpath in glob.glob(evaluation_dir+ '/result-*')]

  p1logs, p2logs = [], []  # [(Sequence of Pick, Win or Lose), ... ]
  for p1log_path, p2log_path, result_path in zip(p1log_paths, p2log_paths, 
                                                 result_paths):
    p1log = read_log(p1log_path)
    p2log = read_log(p2log_path)
    result = read_result(result_path)
    if p1log and p2log and result:
      p1logs.append(recDotDict({'episode': p1log, 'win_or_lose': result[0]}))
      p2logs.append(recDotDict({'episode': p2log, 'win_or_lose': result[1]}))
  return p1logs, p2logs, epoch


def analyse(logs, cardlist):
  histogram = recDotDict()
  for key in cardlist[0].keys():
    histogram[key] = defaultdict(int)

  wins = 0
  for log in logs:
    for p in log.episode.pick:
      histogram.card_id[p] += 1
    wins += log.win_or_lose

  for card_id, freq in histogram.card_id.items():
    for prop, value in cardlist[card_id].items():
      if prop == 'abilities':
        for ability_type in value:
          histogram[prop][ability_type] += freq
      else:
        histogram[prop][value] += freq

  return histogram, wins

def read_cardlist(fpath):
  def read_abilities(ability_str):
    if ability_str == '------':
      abilities = ['_None']
    else:
      abilities = [c for c in ability_str if c != '-'] 
    return  abilities #[1 if char != '-' else 0 for char in ability_str]

  # header: Name  Type	 Cost	 Damage	 Health	   Abilities	 PlayerHP	EnemyHP	CardDraw	Text description
  cardlist = []
  for l in open(fpath):
    card_info = [x.strip() for x in l.split(';')]
    card_id, name, ctype, cost, damage, health, abilities, playerhp, enemyhp, carddraw, _ = card_info
    
    abilities = read_abilities(abilities)
    cardlist.append(recDotDict({
      'card_id': int(card_id) - 1,
      'name': name, 
      'ctype': ctype,
      'cost': int(cost),
      'damage': int(damage),
      'health': int(health),
      'abilities': abilities,
      'playerhp': int(playerhp),
      'enemyhp': int(enemyhp),
      'carddraw': int(carddraw),
    }))
  return cardlist

def get_figure(remove_ticks=False):
  fig = plt.figure(figsize=(8, 6))
  fig.subplots_adjust(wspace=0.2, hspace=0.6)
  main_ax = fig.add_subplot(1, 1, 1)
  if remove_ticks:
    main_ax.spines['top'].set_color('none')
    main_ax.spines['bottom'].set_color('none')
    main_ax.spines['left'].set_color('none')
    main_ax.spines['right'].set_color('none')
    main_ax.tick_params(labelcolor='w', top='off', bottom='off', left='off', right='off')
  return fig, main_ax

def visualize(p1stat, p2stat, output_dir):
  card_props = list(p1stat.keys()) #['card_id', 'cost', 'ctype']
  alpha = 0.5
  #histtype = 'stepfilled'#'stepfilled' #{'bar', 'barstacked', 'step', 'stepfilled'},
  histtype = 'stepfilled'#'stepfilled' #{'bar', 'barstacked', 'step', 'stepfilled'},
  def reshape(key):
    if type(key) == str:
      return key
    key = '%03d' % int(key)
    return key

  for i, prop in enumerate(card_props):
    fig, main_ax = get_figure()
  
    p1data = sorted([(k, v) for k, v in p1stat[prop].items()], key=lambda x: x[0])
    p2data = sorted([(k, v) for k, v in p2stat[prop].items()], key=lambda x: x[0])
    data = [flatten([[reshape(k) for _ in range(v)] for k, v in p1data]),
            flatten([[reshape(k) for _ in range(v)] for k, v in p2data])]
    xlower = min(set(flatten(data)))
    xupper = max(set(flatten(data)))
    all_keys = set(flatten(data))
    bins = len(all_keys)
    data[0] = data[0] + [k for k in all_keys]
    data[1] = data[1] + [k for k in all_keys]

    colors = ['red', 'blue']
    labels = ['NN-based', 'Rule-based']
    sub_ax = fig.add_subplot(1, 1, 1)  
    #sub_ax.set_xlim(xlower, xupper)


    for j in range(2):
      #sub_ax = fig.add_subplot(1, 2, j+1)  
      _histtype = 'step' if prop == 'cost' else histtype
      sub_ax.hist(data[j], color=colors[j], label=labels[j],
                  alpha=alpha, histtype=_histtype, bins=bins)
      #if j != 0: 
      #  sub_ax.tick_params(labelcolor='w', left='off', bottom ='on')


    #sub_ax.text(-0.45, 10, prop)
    main_ax.set_title(prop)
    main_ax.legend()
    fig.savefig(output_dir + '/%s.png' % prop, bbox_inches="tight")#, pad_inches=0.15)
  #exit(1)

def main(args):
  cardlist = read_cardlist(args.card_list_path)
  sente_p1, gote_p2, epoch = read_logs(os.path.join(args.model_dir, 'evaluation/sente'), args.epoch)
  sente_p2, gote_p1, epoch = read_logs(os.path.join(args.model_dir, 'evaluation/gote'), args.epoch)
  p1logs = sente_p1 + gote_p1
  p2logs = sente_p2 + gote_p2

  p1hist, p1wins = analyse(p1logs, cardlist)
  p2hist, p2wins = analyse(p2logs, cardlist)
  if not args.output_dir:
    output_dir = os.path.join(args.model_dir, 'evaluation/visualize/%03d' % epoch)
    if not os.path.exists(output_dir):
      os.makedirs(output_dir)

  with open(output_dir + '/NNBase.stat', 'w') as f:
    sys.stdout = f
    print('<p1 statistics>')
    pprint(p1hist)
    print('')
    print('<Favorite cards>')
    print('Card-id, freq =', sorted(p1hist.card_id.items(), key=lambda x: -x[1]))
    sys.stdout = sys.__stdout__

  with open(output_dir + '/RuleBase.stat', 'w') as f:
    sys.stdout = f
    print('<p2 statistics>')
    pprint(p2hist)
    print('<Favorite cards>')
    print('Card-id, freq =', sorted(p2hist.card_id.items(), key=lambda x: -x[1]))
    sys.stdout = sys.__stdout__

  visualize(p1hist, p2hist, output_dir)



if __name__ == "__main__":
  desc = ""
  parser = argparse.ArgumentParser(description=desc)
  parser.add_argument('model_dir', help ='')
  parser.add_argument('--output_dir', default=None, help ='')
  parser.add_argument('--epoch', default=None, type=int, help ='')
  parser.add_argument('--card_list_path', default='logAnalyzer/cardlist.txt', 
                      help ='')
  args = parser.parse_args()
  main(args)
