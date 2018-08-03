# coding: utf-8
import sys, os, random, copy, socket, time, re, argparse
from collections import OrderedDict
from pprint import pprint
import core.models
import tensorflow as tf
import numpy as np

from core.utils import common, tf_utils
from core.manager import ManagerBase

class ExperimentManager(ManagerBase):
  def __init__(self, args, sess):
    super().__init__(args, sess)
    self.model = None

  @common.timewatch()
  def create_model(self, config, checkpoint_path=None):
    model_type = getattr(core.models, config.model_type)
    if not self.model:
      self.model = model_type(self.sess, config) # Define computation graph

    if not checkpoint_path or not os.path.exists(checkpoint_path + '.index'):
      ckpt = tf.train.get_checkpoint_state(self.checkpoints_path)
      checkpoint_path = ckpt.model_checkpoint_path if ckpt else None

    self.saver = tf.train.Saver(tf.global_variables(), 
                                max_to_keep=config.max_to_keep)
    self.summary_writer = tf.summary.FileWriter(self.summaries_path, 
                                                self.sess.graph)
    if checkpoint_path and os.path.exists(checkpoint_path + '.index'):
      sys.stdout.write("Reading model parameters from %s\n" % checkpoint_path)
      self.saver.restore(self.sess, checkpoint_path)
    else:
      sys.stdout.write("Created model with fresh parameters.\n")
      tf.global_variables_initializer().run()

  def output_variables_as_text(self):
    pass

  def train(self):
    model = self.create_model(self.config)

def main(args):
  tf_config = tf.ConfigProto(
    log_device_placement=False, # 各デバイスへの演算配置の表示
    allow_soft_placement=True,  # GPU上で実行できない演算を自動でCPUに
    gpu_options=tf.GPUOptions(
      allow_growth=True, # True->必要になったら確保, False->全部
    )
  )

  with tf.Graph().as_default(), tf.Session(config=tf_config) as sess:
    tf.set_random_seed(0)
    manager = ExperimentManager(args, sess)
    getattr(manager, args.mode)()


if __name__ == "__main__":
  desc = ""
  parser = argparse.ArgumentParser(description=desc)
  parser.add_argument('model_root_path', type=str, help ='')
  parser.add_argument('mode', type=str, help ='')
  parser.add_argument('-ct','--config_type', default='main', 
                      type=str, help ='')
  parser.add_argument('-cp','--config_path', default='experiments.conf',
                      type=str, help ='')
  parser.add_argument('--cleanup', default=False,
                      type=common.str2bool, help ='')
  args = parser.parse_args()
  main(args)
