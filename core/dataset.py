# coding: utf-8
from core.utils.common import flatten_batch, batching_dicts, recDotDict
import subprocess

class HSReplayDataset(object):
  def __init__(self, config):
    self.dataset_path = config.dataset_path
    pass

  def read_data(self, path):
    pass 

  def get_batches(self, batch_size, iterations_per_epoch, current_epoch, 
                  is_training=True):
    dataset_path = os.path.join(self.dataset_path, current_epoch)
    data = self.read_data(dataset_path )
    pass
