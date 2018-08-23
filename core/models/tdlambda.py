# coding: utf-8
import tensorflow as tf
import numpy as np
from pprint import pprint
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint, dotDict, recDotDefaultDict
from core.models import ModelBase, NStepTD

NUM_CANDIDATES = 3
NUM_TURNS = 30

class TDlambda(NStepTD):
  def __init__(self, sess, config):
    self.td_lambda = config.td_lambda
    super().__init__(sess, config)

  def calc_loss(self, is_end_state, reward, expected_next_q_values,
                q_values_of_selected_action):
    targets = [] 
    for num_step in range(1, NUM_TURNS-1):
      with tf.name_scope('get_target_values%02d' % num_step):
        # あるターンからnum_step先読みしたときのtarget values
        target_values_after_n_step = self.get_target_values(expected_next_q_values, is_end_state, reward, num_step) # [NUM_TURNS, batch_size]
        target_values_after_n_step = [(1.0 - self.td_lambda) * (self.td_lambda ** num_step) * x for x in target_values_after_n_step]
        targets.append(target_values_after_n_step)

    targets = [list(x) for x in zip(*targets)]
    targets = [tf.reduce_sum(x, axis=0)for x in targets]

    with tf.name_scope('loss'):
      losses = []
      for i in range(NUM_TURNS):
        loss = self._clipped_loss(targets[i], q_values_of_selected_action[i])
        losses.append(loss)
      loss = tf.reduce_mean(losses)
    return loss
