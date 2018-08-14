# coding: utf-8
import tensorflow as tf
import numpy as np
from pprint import pprint
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint, dotDict, recDotDefaultDict
from core.models import ModelBase

NUM_CANDIDATES = 3

class BagOfCards(ModelBase):
  def __init__(self, sess, config):
    super().__init__(sess, config)
    self.activation_f = getattr(tf.nn, config.activation)
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
        shape=[None, config.vocab_size.card * (config.max_num_card+1)])
      self.ph.candidates = tf.placeholder(
        tf.int32, name='ph.next_state',
        shape=[None, NUM_CANDIDATES])

      self.ph.next_state = tf.placeholder(
        tf.float32, name='ph.next_state',
        shape=[None, config.vocab_size.card * (config.max_num_card+1)])

      self.ph.next_candidates = tf.placeholder(
        tf.int32, name='ph.next_state',
        shape=[None, config.num_next_candidates_samples, NUM_CANDIDATES])

      self.ph.action = tf.placeholder(tf.int32, name='ph.action', shape=[None])
      self.ph.reward = tf.placeholder(tf.float32, name='ph.reward', shape=[None])
      self.ph.is_end_state = tf.placeholder(
        tf.bool, name='ph.is_end_state', shape=[None])

    with tf.name_scope('keep_prob'):
      self.keep_prob = 1.0 - tf.to_float(self.ph.is_training) * config.dropout_rate

    self.q_values = self.inference(self.ph.state) # [batch_size, vocab_size] 

    # The Q values only of the action chosen in the current step.
    with tf.name_scope('dynamic_batch_size'):
      batch_size = shape(self.ph.state, 0)
    
    # Clip Q values.
    #self.q_values = tf.clip_by_value(self.q_values, 0.0, 1.0)

    with tf.name_scope('q_values_of_selected_action'):
      self.q_values_of_selected_action = tf.reshape(
        batch_gather(self.q_values, self.ph.action), [batch_size]) 

    with tf.name_scope('next_state'):
      next_state = self.ph.next_state

    with tf.name_scope('next_q_values'):
      self.next_q_values = self.inference(next_state) # [batch_size, vocab_size]

      # Clip Q values.
      #self.next_q_values = tf.clip_by_value(self.next_q_values, 0.0, 1.0)


      with tf.name_scope('mask_by_next_candidates'):
        next_candidates_mask = tf.one_hot(self.ph.next_candidates, config.vocab_size.card) # [batch_size, num_next_candidates_samples, NUM_CANDIDATES, vocab_size]
        next_candidates_mask = tf.reduce_sum(next_candidates_mask, axis=2) # [batch_size, num_next_candidates_samples, vocab_size]
        tiled_q_values = tf.tile(
          tf.expand_dims(self.next_q_values, 1), 
          tf.constant([1, config.num_next_candidates_samples, 1,])
        ) # [batch_size, num_next_candidates_samples, vocab_size]

        # Mask q_values.
        masked_next_q_values = next_candidates_mask * tiled_q_values
        # masked_next_q_values = tiled_q_values # このマスクを掛けないとlossがどんどん上がっていく

        self.masked_next_q_values = masked_next_q_values

      # Take the maximum q-values by each of the sampled 3 candidates, and average them.
      with tf.name_scope('expected_next_q_value'):
        self.expected_next_q_value = tf.reduce_mean(tf.reduce_max(masked_next_q_values, axis=-1), axis=-1)



    # Target is the sum of and the intermediate reward and the next Q values discounted by gamma if the next state is not an end state. 
    with tf.name_scope('target_value'):
      # The next expected Q-value is ignored if this step is end state.
      is_end_state_mask = tf.cast(tf.logical_not(self.ph.is_end_state), tf.float32) # [batch_size]
      target = self.ph.reward + self.td_gamma * is_end_state_mask * self.expected_next_q_value # r + gamma * max(Q(s[t+1], a[t+1])) if t+1 != T else r
      self.target = tf.stop_gradient(target)

    #self.loss = self._loss(target, filtered_q_values)
    with tf.name_scope('loss'):
      self.loss = self._clipped_loss(self.target, self.q_values_of_selected_action)
      #self.loss = self._loss(self.target, self.q_values_of_selected_action)
    self.updates = self.get_updates(self.loss, self.global_step)

  def _clipped_loss(self, target, filtered_qs):
    error = tf.abs(target - filtered_qs)
    quadratic = tf.clip_by_value(error, 0.0, 1.0)
    linear = error - quadratic
    return tf.reduce_sum(0.5 * tf.square(quadratic) + linear)

  def _loss(self, target, filtered_qs):
    return tf.reduce_mean(tf.nn.l2_loss(target - filtered_qs))

  def get_input_feed(self, batch):
    input_feed = {}
    input_feed[self.ph.state] = batch.state
    input_feed[self.ph.is_training] = batch.is_training
    if batch.next_state:
      input_feed[self.ph.next_state] = batch.next_state
      input_feed[self.ph.next_candidates] = batch.next_candidates

    if batch.reward  and batch.is_end_state:
      input_feed[self.ph.reward] = batch.reward
      input_feed[self.ph.is_end_state] = batch.is_end_state
    if batch.action:
      input_feed[self.ph.action] = batch.action
    return input_feed

  def inference(self, state):
    with tf.variable_scope('Inference', reuse=tf.AUTO_REUSE):
      x = state
      for i in range(self.num_ff_layers):
        with tf.variable_scope('Forward%d' % (i+1)) as scope:
          x = linear(x, output_size=self.hidden_size, 
                     activation=self.activation_f, scope=scope)
          x = tf.nn.dropout(x, keep_prob=self.keep_prob)
      with tf.variable_scope('Output') as scope:
        q_values = linear(x, output_size=self.vocab_size.card,
                          activation=self.activation_f, scope=scope)
    return q_values
  
  def step(self, batch, step):
    input_feed = self.get_input_feed(batch)

    #self.debug_ops = [self.q_values_of_selected_action,  self.masked_next_q_values, self.expected_next_q_value, self.target, self.ph.reward]
    self.debug_ops = [
      self.q_values_of_selected_action,  
      #self.masked_next_q_values, 
      self.expected_next_q_value, 
      #self.target, self.ph.reward,
    ]
    #self.debug_ops = []
    if self.debug_ops:
      for k, v in zip(self.debug_ops, self.sess.run(self.debug_ops, input_feed)):
        print ('Step %d' % step)
        print(k, v)
        #print(v)
        #print ()
      #if step == 1000:
      #  exit(1)

    output_feed = [self.q_values]
    if batch.is_training:
      output_feed += [self.loss, self.updates]
    outputs = self.sess.run(output_feed, input_feed)
    return outputs

