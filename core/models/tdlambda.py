# coding: utf-8
import tensorflow as tf
import numpy as np
from pprint import pprint
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint, dotDict, recDotDefaultDict
from core.models import ModelBase
NUM_CANDIDATES = 3
NUM_TURNS = 30

class TDlambda(NStepTD):
  def __init__(self, sess, config):
    super().__init__(sess, config)
    self.hidden_activation = getattr(tf.nn, config.hidden_activation)
    self.output_activation = getattr(tf.nn, config.output_activation)
    #self.activation_f = tf.nn.sigmoid
    self.hidden_size = config.hidden_size
    self.num_ff_layers = config.num_ff_layers
    self.vocab_size = config.vocab_size
    self.max_num_card = config.max_num_card
    self.td_gamma = config.td_gamma

    # Define placeholders.
    with tf.name_scope('Placeholders'):
      self.ph = dotDict()
      self.ph.is_training = tf.placeholder(tf.bool, name='is_training', shape=[])
      self.ph.state = tf.placeholder(
        tf.float32, name='ph.state',
        shape=[None, NUM_TURNS, Nconfig.vocab_size.card * (config.max_num_card+1)])
      self.ph.candidates = tf.placeholder(
        tf.int32, name='ph.candidates',
        shape=[None, NUM_TURNS, NUM_CANDIDATES])

      self.ph.next_state = tf.placeholder(
        tf.float32, name='ph.next_state',
        shape=[None, NUM_TURNS, config.vocab_size.card * (config.max_num_card+1)])

      self.ph.next_candidates = tf.placeholder(
        tf.int32, name='ph.next_candidates',
        shape=[None, NUM_TURNS, config.num_next_candidates_samples, NUM_CANDIDATES])

      self.ph.action = tf.placeholder(tf.int32, name='ph.action', shape=[None, NUM_TURNS])
      self.ph.reward = tf.placeholder(tf.float32, name='ph.reward', shape=[None, NUM_TURNS])
      self.ph.is_end_state = tf.placeholder(
        tf.bool, name='ph.is_end_state', shape=[None, NUM_TURNS])

    with tf.name_scope('keep_prob'):
      self.keep_prob = 1.0 - tf.to_float(self.ph.is_training) * config.dropout_rate
