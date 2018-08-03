# coding: utf-8
from core.utils.common import flatten_batch, batching_dicts, recDotDict

class HSReplayDataset(object):
  def __init__(self, config):
    pass

  def read_data(self, path):
    pass 

  def get_batches(self, batch_size, iterations_per_epoch, is_training=True):
    data_path = 'aa'
    data = self.read_data(data_path)
    pass
