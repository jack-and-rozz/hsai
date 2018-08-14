# coding: utf-8
import sys, os, random, copy, socket, time, re, argparse
from collections import OrderedDict
from pprint import pprint
import core.dataset
#from core.dataset import HSReplayDataset
import core.models
import tensorflow as tf
import numpy as np

from core.utils import common, tf_utils
from core.manager import ManagerBase

class ExperimentManager(ManagerBase):
  def __init__(self, args, sess):
    super().__init__(args, sess)
    self.model = None
    #self.dataset = HSReplayDataset(self.replays_path, self.config.dataset)
    dataset_type = getattr(core.models, config.dataset.dataset_type)
    self.dataset = HSReplayDataset(self.replays_path, self.config.dataset)

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

    variables_path = self.root_path + '/variables.list'
    with open(variables_path, 'w') as f:
      variable_names = sorted([v.name + ' ' + str(v.get_shape()) for v in tf.global_variables()])
      f.write('\n'.join(variable_names) + '\n')
    return self.model

  def output_variables_as_text(self, model):
    epoch = model.epoch.eval()
    variables_dir = self.variables_path + '/%03d' % epoch
    if not os.path.exists(variables_dir):
      os.makedirs(variables_dir)
    for v in tf.global_variables():
      name = v.name
      if re.search('Adam', name):
        continue

      if re.search('Inference', name):
        name = v.name.replace('/', '-').replace(':0', '') + '.csv'
        variables_path = os.path.join(variables_dir, name)
        np.savetxt(variables_path, v.eval())

  def save_model(self, model, save_as_best=False):
    checkpoint_path = self.checkpoints_path + '/model.ckpt'
    self.saver.save(self.sess, checkpoint_path, global_step=model.epoch)
    if save_as_best:
      suffixes = ['data-00000-of-00001', 'index', 'meta']
      for sfx in suffixes:
        source_path = self.checkpoints_path + "/model.ckpt-%d.%s" % (model.epoch.eval(), sfx)
        target_path = self.checkpoints_path + "/%s.%s" % (BEST_CHECKPOINT_NAME, sfx)
        if os.path.exists(source_path):
          cmd = "cp %s %s" % (source_path, target_path)
          os.system(cmd)

  def train(self):
    model = self.create_model(self.config)
    
    for epoch in range(model.epoch.eval(), self.config.max_epoch):
      sys.stdout.write('Save the model at the begining of epoch %02d as model.ckpt-%d\n' % (epoch, epoch))

      self.save_model(model)
      self.output_variables_as_text(model)

      batches = self.dataset.get_batches(
        self.config.batch_size, model.epoch.eval(), is_training=True)

      average_loss = 0.0
      for i, batch in enumerate(batches):
        q_values, loss, _ =  model.step(batch, i)
        average_loss += loss
        print('step = %d, loss = %f' % (i, loss))
      average_loss /= (i+1)
      self.logger.info('Epoch %d, Average loss = %f' % (epoch, average_loss))
      summary_dict = {}
      summary_dict["train/loss"] = average_loss
      summary = tf_utils.make_summary(summary_dict)
      self.summary_writer.add_summary(summary, model.epoch.eval())

      model.add_epoch()

  def test(self, model=None):
    if not model:
      model = self.create_model(self.config)
    batch = common.recDotDefaultDict()
    state = [[1.0 for _ in range(640)]]
    batch.state = state
    batch.is_training = False
    res = model.step(batch, 0)
    print (res)
    
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
  '''
  '''
  desc = "Based on https://github.com/yuishihara/DQN-tensorflow/tree/dqn_nature_ver"
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
