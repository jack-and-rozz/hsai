# coding: utf-8
import tensorflow as tf
from pprint import pprint
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint, dotDict, recDotDefaultDict
from core.models import ModelBase

class BagOfCards(ModelBase):
  def __init__(self, sess, config):
    super().__init__(sess, config)
    self.activation_f = tf.nn.relu
    self.hidden_size = config.hidden_size
    self.num_ff_layers = config.num_ff_layers
    self.vocab_size = config.vocab_size
    self.gamma = config.gamma

    # Define placeholders.
    with tf.name_scope('Placeholders'):
      self.ph = dotDict()
      self.ph.is_training = tf.placeholder(tf.bool, name='is_training', shape=[])
      self.ph.state = tf.placeholder(tf.float32, name='ph.state',
                                     shape=[None, config.vocab_size.card])
      self.ph.action = tf.placeholder(tf.int32, name='ph.action',
                                      shape=[None])
      self.ph.reward = tf.placeholder(tf.float32, name='ph.reward', shape=[None])
      self.ph.is_end_state = tf.placeholder(tf.bool, name='ph.is_end_state', 
                                            shape=[None])


    with tf.name_scope('keep_prob'):
      self.keep_prob = 1.0 - tf.to_float(self.ph.is_training) * config.dropout_rate

    self.q_values = self.inference(self.ph.state) # [batch_size, vocab_size] 

    # The Q values only of the action chosen in the current step.
    
    with tf.name_scope('dynamic_batch_size'):
      batch_size = shape(self.ph.state, 0)

    with tf.name_scope('filtered_q_values'):
      filtered_q_values = tf.reshape(
        batch_gather(self.q_values, self.ph.action), [batch_size]) 

    with tf.name_scope('next_state'):
      next_state = self.ph.state + tf.one_hot(self.ph.action, config.vocab_size.card)
    with tf.name_scope('next_q_values'):
      next_q_values = self.inference(next_state) # [batch_size, vocab_size]
      next_q_value_mask = tf.cast(tf.logical_not(self.ph.is_end_state), tf.float32) # [batch_soize]

    # Target is the sum of and the intermediate reward and the next Q values discounted by gamma if the next state is not an end state. 
    with tf.name_scope('target_value'):
      target = self.ph.reward + self.gamma * next_q_value_mask * tf.reduce_max(next_q_values, axis=1) # r + gamma * max(Q(s[t+1], a[t+1])) if t+1 != T else r
      target = tf.stop_gradient(target)

    #self.loss = self._loss(target, filtered_q_values)
    with tf.name_scope('loss'):
      self.loss = self._clipped_loss(target, filtered_q_values)
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
                          activation=tf.nn.relu, scope=scope)
    return q_values
  
  def step(self, batch):
    input_feed = self.get_input_feed(batch)
    output_feed = [self.q_values]
    if batch.is_training:
      output_feed += [self.loss, self.updates]
    outputs = self.sess.run(output_feed, input_feed)
    return outputs

