from agent import Agent
import numpy as np
import tensorflow as tf
from tensorflow.python.ops import gradients


class RLAgent(Agent):
    def __init__(self, args, client, global_model):
        super().__init__(args)
        self._global_model = global_model
        #self._local_model = global_model.clone_model()
        
    def start_episode(self):
        pass
