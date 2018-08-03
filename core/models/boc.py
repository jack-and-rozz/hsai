# coding: utf-8
import tensorflow as tf
from core.utils.tf_utils import linear, batch_gather, shape
from core.utils.common import dbgprint
from core.models import ModelBase

class BagOfCards(ModelBase):
  def __init__(self, sess, config):
    super().__init__(sess, config)
    self.activation_f = tf.nn.relu
    self.hidden_size = config.hidden_size
    self.num_ff_layers = config.num_ff_layers
    self.vocab_size = config.vocab_size
    self.gamma = config.gamma

    with tf.name_scope('Placeholders'):
      batch_size = None
      self.is_training = tf.placeholder(tf.bool, name='is_training', shape=[])
      self.state = tf.placeholder(tf.float32, [batch_size, config.vocab_size.card])

      self.action_filter = tf.placeholder(tf.float32, [batch_size, config.vocab_size.card]) # 1 for the dimensions of dealt cards and 0 for else. Only for testing.
      self.action = tf.placeholder(tf.int32, [batch_size])
      self.target = tf.placeholder(tf.float32, [batch_size])
      batch_size = shape(self.state, 0)

    self.keep_prob = 1.0 - tf.to_float(self.is_training) * config.dropout_rate
    self.q_values = self.inference(self.state)

    #self.filtered_q_values = batch_gather(self.q_values, action_filter)
    self.filtered_q_values = tf.reshape(batch_gather(self.q_values, self.action), 
                                        [batch_size])
    self.loss = 
    dbgprint(self.q_values)
    dbgprint(self.filtered_q_values)
    exit(1)

    with tf.variable_scope('BagOfCards'):
      pass

  def clipped_loss(self, data, filtered_qs):
    error = tf.abs(target - filtered_qs)
    quadratic = tf.clip_by_value(error, 0.0, 1.0)
    linear = error - quadratic
    return tf.reduce_sum(0.5 * tf.square(quadratic) + linear)

  def loss(self, target, filtered_qs):
    return tf.reduce_mean(tf.nn.l2_loss(target - filtered_qs))

  def get_input_feed(self, batch):
    input_feed = {}
    input_feed[self.state] = batch.state
    if batch.reward and batch.is_end_state is not None:
      batch_size = len(batch.reward)
      target = np.zeros([batch_size])
      for i in range(len(batch_size)):
        target[i] = 
      input_feed[self.target] = batch.reward if 

    if batch.action:
      input_feed[self.action] = batch.action
    if batch.action_filter:
      input_feed[self.action_filter] = batch.action_filter
    pass


  def inference(self, state):
    with tf.name_scope('Inference'):
      x = state
      for i in range(self.num_ff_layers):
        with tf.variable_scope('Forward%d' % (i+1)) as scope:
          x = linear(x, output_size=self.hidden_size, 
                     activation=self.activation_f, scope=scope)

      with tf.variable_scope('Output') as scope:
        q_values = linear(x, output_size=self.vocab_size.card,
                          activation=tf.nn.relu, scope=scope)
    return q_values


