# coding: utf-8
import tensorflow as tf
from core.utils.tf_utils import linear


class BagOfCards(ModelBase):
  def __init__(self, sess, config):
    super().__init__(sess, config)
    self.activation_f = tf.nn.relu
    self.hidden_size = config.hidden_size
    self.num_ff_layers = config.num_ff_layers
    self.keep_prob = 1.0 - tf.to_float(self.is_training) * config.dropout_rate

    with tf.name_scope('Placeholders'):
      batch_size = None
      self.is_training = tf.placeholder(tf.bool, name='is_training', shape=[])
      self.state = tf.placeholder(tf.float32, [batch_size, config.vocab_size.card])

    with tf.name_scope('Inference'):
      x = self.state
      for i in range(config.z):
        with tf.variable_scope('Forward%d' % i) as scope:
          x = linear(x, output_size=self.hidden_size, 
                     activation=self.activation_f, scope=scope)

      with tf.variable_scope('Output') as scope:
        self.q_values = linear(x, output_size=config.vocab_size.card,
                               activation=tf.nn.relu, scope=scope)


