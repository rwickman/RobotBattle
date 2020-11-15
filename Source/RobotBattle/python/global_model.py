import datetime, json, os, multiprocessing
import numpy as np
import tensorflow as tf

from actor_critic import ActorCritic

class GlobalModel:
    def __init__(self, args):
        self._args = args
        self._train_lock = multiprocessing.Lock()
        self._global_training_json = "model/global_training.json"
        self.model_weights_dir = "model/actor_critic"
        self.optim_ckpt_dir = "model/optim_ckpts"
        self.setup_model()

    def setup_model(self):
        """Setup up the ActorCritic model and optimizers."""
        self._build_model()
        self._create_checkpoint()
        if self._args.load_model:
            self.load_model_weights()
        else:
            self._global_train_data = {"rl_episodes" : 0}
        
        if self._args.episodes > 0:
            self._create_summary_writers()
  
    def clone(self):
        """Clone the global model."""
        cloned_model = ActorCritic(self._args)
        if self._args.load_model:
            cloned_model.load_weights(self.model_weights_dir)
        #cloned_model.set_weights(self._actor_critic.get_weights())
        return cloned_model
    

    def _build_model(self):
        self._actor_critic = ActorCritic(self._args)
        self._optimizer = tf.keras.optimizers.Adam(learning_rate=self._args.lr)
        # Call the model on fake data to build it
        fake_state = np.zeros((1, 6, 9))
        self._actor_critic(fake_state)
        self._actor_critic.summary()
          #self._actor_critic.summary()

    def _create_checkpoint(self):
        """ Saves the state of the optimizer"""
        self.ckpt = tf.train.Checkpoint(optimizer=self._optimizer)
        self.manager = tf.train.CheckpointManager(self.ckpt, self.optim_ckpt_dir , max_to_keep=3)
    
    def _save_model_weights(self):
        self.manager.save() 
        self._actor_critic.save_weights(self.model_weights_dir)
        
        with open(self._global_training_json, "w") as f:
            json.dump(self._global_train_data, f)
    
    def load_model_weights(self):
        print("LOADING MODEL")
        # Load model weights
        self._actor_critic.load_weights(self.model_weights_dir)
        
        # Restore optimizer states
        self.ckpt.restore(self.manager.latest_checkpoint)
        self._optimizer.learning_rate = self._args.lr
        
        # Load train data
        if os.path.exists(self._global_training_json): 
            with open(self._global_training_json, "r") as f:
                self._global_train_data = json.load(f)
        else:
            self._global_train_data = {"rl_episodes" : 0}


    def apply_gradients(self, grads):
        with self._train_lock:
            self._optimizer.apply_gradients(zip(grads, self._actor_critic.trainable_variables))
            return self._actor_critic.get_weights()

    def _create_summary_writers(self):   
        #current_time = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
        actor_log_dir= "logs/fit/actor"
        critic_log_dir= "logs/fit/critic"
        returns_log_dir= "logs/fit/returns"
        self._actor_summary_writer = tf.summary.create_file_writer(actor_log_dir)
        self._critic_summary_writer = tf.summary.create_file_writer(critic_log_dir)
        self._returns_summary_writer = tf.summary.create_file_writer(returns_log_dir)
    
    def save_rl_episode(self, avg_actor_loss, avg_critic_loss, avg_return):
        with self._train_lock:
            # Write out episode metrics
            self._write_rl_agent_summary(avg_actor_loss, avg_critic_loss, avg_return)
            
            # Increment number of episodes
            self._global_train_data["rl_episodes"] += 1
            
            # Save model weights
            self._save_model_weights()
    
    def _write_rl_agent_summary(self, avg_actor_loss, avg_critic_loss, avg_return):        
        with self._actor_summary_writer.as_default():
            tf.summary.scalar("Avg Actor loss", avg_actor_loss, step=self._global_train_data["rl_episodes"])
        with self._critic_summary_writer.as_default():
            tf.summary.scalar("Avg Critic loss", avg_critic_loss, step=self._global_train_data["rl_episodes"])
        with self._returns_summary_writer.as_default():
            tf.summary.scalar("Avg Return", avg_return, step=self._global_train_data["rl_episodes"])