# coding: utf-8
import tensorflow as tf
import numpy as np
from pprint import pprint
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint, dotDict, recDotDefaultDict, flatten_recdict
from core.models import ModelBase

NUM_CANDIDATES = 3
NUM_TURNS = 30

class NStepTD(ModelBase):
  def __init__(self, sess, config):
    ModelBase.__init__(self, sess, config)
    self.config = config
    self.hidden_activation = getattr(tf.nn, config.hidden_activation)
    self.output_activation = getattr(tf.nn, config.output_activation)
    #self.activation_f = tf.nn.sigmoid
    self.hidden_size = config.hidden_size
    self.num_ff_layers = config.num_ff_layers
    self.vocab_size = config.vocab_size
    self.max_num_card = config.max_num_card
    self.td_gamma = config.td_gamma
    self.num_step = config.num_step
    
    self.ph = self.setup_placeholders(config)

    with tf.name_scope('keep_prob'):
      self.keep_prob = 1.0 - tf.to_float(self.ph.is_training) * config.dropout_rate
    with tf.name_scope('merge_meta_features'):
      state = tf.concat([
        self.ph.state, self.ph.is_sente, 
        tf.expand_dims(self.ph.current_num_cards, -1)
      ], axis=-1)
      next_state = tf.concat([
        self.ph.next_state, self.ph.is_sente, 
        tf.expand_dims(self.ph.current_num_cards + 1, -1)
      ], axis=-1)
      state = tf.cast(state, tf.float32)
      next_state = tf.cast(next_state, tf.float32)

    with tf.name_scope('get_q_values'):
      state = tf.unstack(state, axis=1)
      next_state = tf.unstack(next_state, axis=1)
      action = tf.unstack(self.ph.action, axis=1)
      next_candidates = tf.unstack(self.ph.next_candidates, axis=1)
      reward = tf.unstack(self.ph.reward, axis=1)
      is_end_state = tf.unstack(self.ph.is_end_state, axis=1)

      q_values = []
      q_values_of_selected_action = []
      expected_next_q_values = []
      for s, a, ns, nc in zip(state, action, next_state, next_candidates):
        qv, qvsa, next_qv = self.get_q_values(s, a, ns, nc)
        q_values.append(qv)
        q_values_of_selected_action.append(qvsa)
        expected_next_q_values.append(next_qv)

      self.q_values = q_values 

    with tf.name_scope('calc_loss'):
      self.loss = self.calc_loss(is_end_state, reward, expected_next_q_values, 
                            q_values_of_selected_action)
    self.updates = self.get_updates(self.loss, self.global_step)

  def calc_loss(self, is_end_state, reward, expected_next_q_values,
                       q_values_of_selected_action):
    with tf.name_scope('get_target_values'):
      targets = self.get_target_values(expected_next_q_values, is_end_state, 
                                       reward, self.num_step)
    with tf.name_scope('loss'):
      losses = []
      for i in range(NUM_TURNS):
        loss = self._clipped_loss(targets[i], q_values_of_selected_action[i])
        losses.append(loss)
      loss = tf.reduce_mean(losses)
    return loss

  def get_q_values(self, state, action, next_state, next_candidates):
    q_values = self.calc_q_values(state) # [batch_size, vocab_size] 

    # The Q values only of the action chosen in the current step.
    with tf.name_scope('dynamic_batch_size'):
      batch_size = shape(state, 0)

    with tf.name_scope('q_values_of_selected_action'):
      q_values_of_selected_action = tf.reshape(
        batch_gather(q_values, action), [batch_size]) 

    with tf.name_scope('next_q_values'):
      next_q_values = self.calc_q_values(next_state) # [batch_size, vocab_size]

      with tf.name_scope('mask_by_next_candidates'):
        next_candidates_mask = tf.one_hot(next_candidates, self.config.vocab_size.card) # [batch_size, num_next_candidates_samples, NUM_CANDIDATES, vocab_size]
        next_candidates_mask = tf.reduce_sum(next_candidates_mask, axis=2) # [batch_size, num_next_candidates_samples, vocab_size]
        tiled_q_values = tf.tile(
          tf.expand_dims(next_q_values, 1), 
          tf.constant([1, self.config.num_next_candidates_samples, 1,])
        ) # [batch_size, num_next_candidates_samples, vocab_size]

        # Mask q_values.
        masked_next_q_values = next_candidates_mask * tiled_q_values
        # masked_next_q_values = tiled_q_values 
      # Take the maximum q-values by each of the sampled 3 candidates, and average them.
      with tf.name_scope('expected_next_q_value'):
        expected_next_q_value = tf.reduce_mean(
          tf.reduce_max(masked_next_q_values, axis=-1), axis=-1)

    return q_values, q_values_of_selected_action, expected_next_q_value

  def get_target_values(self, expected_next_q_value, is_end_state, reward, N):
    '''
    Args:
    expected_next_q_value, is_end_state, reward: Lists of tensors.
    '''
    # Target is the sum of and the intermediate reward and the next Q values discounted by gamma if the next state is not an end state.
    i = 0

    with tf.name_scope('target_value'):
      # The next expected Q-value is ignored if this step is end state.
      targets = []
      for i in range(NUM_TURNS):
        # N step先読みした後のtime step, 0~29 stepまで

        last_step = min(i + N, NUM_TURNS - 1)
        n = last_step - i  # 実際に何ステップ読んだか 
        is_end_state_mask = tf.cast(tf.logical_not(is_end_state[last_step]), 
                                    tf.float32) # [batch_size]

        target = [r * (self.td_gamma ** j) for j, r in enumerate(reward[i:last_step+1])]
        target = tf.reduce_sum(target, axis=0)
        # sum[t:t+N] {r_{t+i} * (gamma ** i)} +(gamma ** (i+1)) * max(Q(s[t+i], a[t+i]))
        target += (self.td_gamma ** n) * is_end_state_mask * expected_next_q_value[last_step]
        target = tf.stop_gradient(target)
        targets.append(target)
    return targets


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
    input_feed[self.ph.is_sente] = batch.is_sente
    input_feed[self.ph.current_num_cards] = batch.current_num_cards
    
    if batch.next_state:
      input_feed[self.ph.next_state] = batch.next_state
      input_feed[self.ph.next_candidates] = batch.next_candidates

    if batch.reward  and batch.is_end_state:
      input_feed[self.ph.reward] = batch.reward
      input_feed[self.ph.is_end_state] = batch.is_end_state
    if batch.action:
      input_feed[self.ph.action] = batch.action
    return input_feed

  def calc_q_values(self, state):
    with tf.variable_scope('Inference', reuse=tf.AUTO_REUSE):
      x = state
      for i in range(self.num_ff_layers):
        with tf.variable_scope('Forward%d' % (i+1)) as scope:
          x = linear(x, output_size=self.hidden_size, 
                     activation=self.hidden_activation, scope=scope)
          x = tf.nn.dropout(x, keep_prob=self.keep_prob)
      with tf.variable_scope('Output') as scope:
        q_values = linear(x, output_size=self.vocab_size.card,
                          activation=self.output_activation,
                          scope=scope)
                          #activation=self.activation_f, scope=scope)
    return q_values

  def setup_placeholders(self, config):
    # Define placeholders.
    with tf.name_scope('Placeholders'):
      ph = dotDict()
      ph.is_training = tf.placeholder(tf.bool, name='is_training', shape=[])
      ph.state = tf.placeholder(
        tf.int32, name='ph.state',
        shape=[None, NUM_TURNS, config.vocab_size.card * (config.max_num_card+1)])

      ph.candidates = tf.placeholder(
        tf.int32, name='ph.next_state',
        shape=[None, NUM_TURNS, NUM_CANDIDATES])

      ph.next_state = tf.placeholder(
        tf.int32, name='ph.next_state',
        shape=[None, NUM_TURNS, config.vocab_size.card * (config.max_num_card+1)])

      ph.next_candidates = tf.placeholder(
        tf.int32, name='ph.next_state',
        shape=[None, NUM_TURNS, config.num_next_candidates_samples, NUM_CANDIDATES])

      ph.action = tf.placeholder(tf.int32, name='ph.action', shape=[None, NUM_TURNS])
      ph.reward = tf.placeholder(tf.float32, name='ph.reward', shape=[None, NUM_TURNS])
      ph.is_end_state = tf.placeholder(
        tf.bool, name='ph.is_end_state', shape=[None, NUM_TURNS])
      ph.is_sente = tf.placeholder(
        tf.int32, name='ph.is_sente', shape=[None, NUM_TURNS, 2])
      ph.current_num_cards = tf.placeholder(
        tf.int32, name='ph.current_num_cards', shape=[None, NUM_TURNS])
    return ph

  def step(self, batch, step):
    input_feed = self.get_input_feed(batch)

    #self.debug_ops = [self.q_values_of_selected_action,  self.masked_next_q_values, self.expected_next_q_value, self.target, self.ph.reward]
    self.debug_ops = [
      self.loss,
      #self.q_values
      #self.masked_next_q_values, 
      #self.expected_next_q_value, 
      #self.target, self.ph.reward,
    ]
    #self.debug_ops = []
    if batch.is_training and self.debug_ops:
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

