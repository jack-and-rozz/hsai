# coding: utf-8
from core.utils.common import flatten_batch, batching_dicts, dotDict, recDotDefaultDict, separate_path_and_filename, dbgprint, flatten
import subprocess, os, re, time, sys, random, copy
import glob
import numpy as np
from pprint import pprint

def add_replay(data, fpath, max_num_card):
  fdir, fname = separate_path_and_filename(fpath)
  m = re.match('result-(.+)', fname)
  fkey = m.group(1)

  # Read player actions and the state there.
  # log = [state, candidate, action, reward, is_end_state]
  p1fname = 'player1-' + fkey
  p1log = read_log(os.path.join(fdir, p1fname), max_num_card)
  p2fname = 'player2-' + fkey
  p2log = read_log(os.path.join(fdir, p2fname), max_num_card)

  if not p1log or not p2log: # Log parse error
    return

  l = open(fpath).readline().split() # Read the result of the game

  # Set the result of the game as reward.
  p1log[-1].reward = int(l[0])
  p2log[-1].reward = int(l[1])
  #p1log[-1].reward = 1 if int(l[0]) == 1 else -1
  #p2log[-1].reward = 1 if int(l[1]) == 1 else -1
  p1log[-1].is_end_state = True
  p2log[-1].is_end_state = True
  try:
    data[fkey] = [p1log, p2log]
  except:
    print(data)
    print(fkey)
    exit(1)
  return data

def state_to_onehot(state, max_num_card):
  # Expand the dimensions of state by max_num_card.
  one_hot_state = [0 for _ in range(len(state) * (max_num_card + 1))]
  for i, n in enumerate(state):
    n = n if n <= max_num_card else max_num_card
    one_hot_state[i * (max_num_card + 1) + n] = 1
  return one_hot_state
    

def read_log(fpath, max_num_card):
  if not os.path.exists(fpath):
    return 
  logs = [l.strip() for l in open(fpath)]
  if len(logs) != 90: # 30 turn * 3
    return None
  n_examples = len(logs) // 3

  data = []
  for i in range(n_examples):
    t = 3 * i
    state = [int(x) for x in logs[t].split()]
    candidates = [int(x) for x in logs[t+1].split()]
    action = int(logs[t+2].split()[0])
    new_state = [int(x) for x in logs[t].split()]
    new_state[action] += 1

    # Temporal values.
    reward = 0
    is_end_state = False
    d = recDotDefaultDict()

    # Convert state format from [num_card0, num_card1, ....] to [card0_0, card0_1, ... card0_max_num_card, card1_0, ...]. Each element must be 0 or 1.
    d.state = state_to_onehot(state, max_num_card)
    d.next_state = state_to_onehot(new_state, max_num_card)
    d.candidates = candidates
    d.action = action
    d.reward = reward
    d.is_end_state = is_end_state
    data.append(d)
  return data

class HSReplayDataset(object):
  def __init__(self, dataset_path, config):
    self.dataset_path = dataset_path
    self.iterations_per_epoch = config.iterations_per_epoch
    self.memory_size = config.memory_size
    self.max_num_card = config.max_num_card
    pass

  def read_data(self, replay_path):
    '''
    Args:
    - replay_path: 
    '''
    while True: # Wait until the number of replays exceeds self.memory_size
      replays = glob.glob(replay_path + '/result-*') 
      n_replays = len(replays) * 60 # 30 * 2 picks at a game.
      if n_replays > self.memory_size:
        break
      sys.stderr.write('No enough replays. (%d/%d)\n' % (n_replays, self.memory_size))
      time.sleep(1)
    replays = replays[:self.memory_size // 60 + 1]
    data = recDotDefaultDict()
    for i, fpath in enumerate(replays):
      if i % (len(replays) // 10) == 0:
        sys.stderr.write('Reading logs from \'%s\' ... (%d/%d)\n' % (replay_path, i, len(replays)))
      add_replay(data, fpath, self.max_num_card)

    flat_data = []
    for d in data.values():
      for x in d:
        flat_data += x
    return flat_data

  def get_batches(self, batch_size, current_epoch, 
                  random_sample=True, is_training=True):
    dataset_path = os.path.join(self.dataset_path, '%03d' % current_epoch)
    data = self.read_data(dataset_path)
    for i in range(self.iterations_per_epoch):
      batch = recDotDefaultDict()
      batch.is_training = is_training 
      if random_sample:
        sampled_data = random.sample(data, batch_size)
        for d in sampled_data:
          batching_dicts(batch, d)
        yield batch
  
