#coding: utf-8
import argparse, os
import numpy as np
import matplotlib.pyplot as plt

def main(args):
  data =[]
  path = args.model_path + '/variables'
  en_w_state = np.loadtxt(path + '/encoder.w_state', delimiter=',')
  en_local_w = en_w_state[:len(en_w_state)/2].flatten()
  en_global_w = en_w_state[len(en_w_state)/2:].flatten()
  data.append((en_local_w, en_global_w))

  if os.path.exists(path + '/decoder.w_state'):
    de_w_state = np.loadtxt(path + '/decoder.w_state', delimiter=',')
    de_local_w = de_w_state[:len(de_w_state)/2].flatten()
    de_global_w = de_w_state[len(de_w_state)/2:].flatten()
    data.append((de_local_w, de_global_w))

  plt.rcParams["font.size"] = 18
  fig = plt.figure(figsize=(6, 6))
  plot(fig, data)
  file_path = args.model_path + '/variables/' + 'w_state.eps'
  fig.savefig(file_path, format='eps')



def plot(fig, data):
  alpha = 0.50
  bins = 500
  normed = True
  xlim, ylim = 0.5, 7.0
  histtype = 'step'
  xlabel, ylabel = 'value', 'normalized frequency'
  n_line = len(data)
  main_ax = fig.add_subplot(1, 1, 1)
  main_ax.spines['top'].set_color('none')
  main_ax.spines['bottom'].set_color('none')
  main_ax.spines['left'].set_color('none')
  main_ax.spines['right'].set_color('none')
  main_ax.tick_params(labelcolor='w', top='off', bottom='off', left='off', right='off')
  subplots = []
  optional_label = ['Encoder', 'Decoder']
  for i, (l, g) in enumerate(data):
    if i == 0:
      sub_ax = fig.add_subplot(2, 1, i+1)
    else:
      sub_ax = fig.add_subplot(2, 1, i+1, sharex=subplots[0])

    subplots.append(sub_ax)
    sub_ax.hist(l, bins=bins, normed=normed, color='red',
            alpha=alpha, histtype=histtype, label='Local')
    sub_ax.hist(g, bins=bins, normed=normed, color='blue',
            alpha=alpha, histtype=histtype, label='Global')
    sub_ax.set_xlim(-xlim, xlim)
    sub_ax.set_ylim(0, ylim)
    sub_ax.text(-0.45, 6, optional_label[i])
    if i == 0:
      sub_ax.legend(fontsize=14)


  if len(data) > 1:
    ax = main_ax
  else:
    ax = subplots[0]
  ax.set_title(args.title)
  ax.set_xlabel(xlabel)
  ax.set_ylabel(ylabel)



if __name__ == "__main__":
    desc = ""
    parser = argparse.ArgumentParser(description=desc)
    parser.add_argument('model_path', help ='')
    parser.add_argument('title', help ='')
    global args
    args = parser.parse_args()
    main(args)
