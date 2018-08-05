# coding: utf-8
import math
import tensorflow as tf

class ModelBase(object):
  def __init__(self, sess, config):
    self.sess = sess
    self.debug_ops = []

    with tf.name_scope('global_variables'):
      self.global_step = tf.get_variable(
        "global_step", trainable=False, shape=[],  dtype=tf.int32,
        initializer=tf.constant_initializer(0, dtype=tf.int32)) 

      self.epoch = tf.get_variable(
        "epoch", trainable=False, shape=[], dtype=tf.int32,
        initializer=tf.constant_initializer(0, dtype=tf.int32)) 

      self.max_score = tf.get_variable(
        "max_score", trainable=False, shape=[],  dtype=tf.float32,
        initializer=tf.constant_initializer(0.0, dtype=tf.float32)) 

    self.optimizer_type = config.optimizer.optimizer_type
    self.learning_rate = config.optimizer.learning_rate
    self.decay_rate = config.optimizer.decay_rate
    self.decay_frequency = config.optimizer.decay_frequency
    self.max_gradient_norm = config.optimizer.max_gradient_norm

    # Define operations in advance not to create ops in the loop.
    with tf.name_scope('add_step'):
      self._add_step = tf.assign(self.global_step, tf.add(self.global_step, tf.constant(1, dtype=tf.int32)))
    with tf.name_scope('add_epoch'):
      self._add_epoch = tf.assign(self.epoch, tf.add(self.epoch, tf.constant(1, dtype=tf.int32)))

    with tf.name_scope('update_max_score'):
      self._next_score = tf.placeholder(tf.float32, name='max_score_ph', shape=[])
      self._update_max_score = tf.assign(self.max_score, self._next_score)

  def initialize_embeddings(self, name, emb_shape, initializer=None, 
                            trainable=True):
    if not initializer:
      initializer = tf.random_uniform_initializer(-math.sqrt(3), math.sqrt(3))
    embeddings = tf.get_variable(name, emb_shape, trainable=trainable,
                                 initializer=initializer)
    return embeddings

  def get_input_feed(self, batch, is_training):
    return {}

  def add_step(self):
    self.sess.run(self._add_step)

  def add_epoch(self):
    self.sess.run(self._add_epoch)

  def update_max_score(self, score):
    self.sess.run(self._update_max_score, feed_dict={self._next_score:score})

  def get_updates(self, loss, global_step):
    with tf.name_scope("update"):
      learning_rate = tf.train.exponential_decay(
        self.learning_rate, global_step,
        self.decay_frequency, self.decay_rate, staircase=True)

      params = tf.contrib.framework.get_trainable_variables()
      opt = getattr(tf.train, self.optimizer_type)(learning_rate)
      if self.max_gradient_norm:
        gradients = [grad for grad, _ in opt.compute_gradients(loss)]
        clipped_gradients, _ = tf.clip_by_global_norm(gradients, 
                                                      self.max_gradient_norm)
        grad_and_vars = [(g, v) for g, v in zip(clipped_gradients, params)]
        updates = opt.apply_gradients(
          grad_and_vars, global_step=global_step)
        return updates
      else:
        return opt.minimize(loss)

