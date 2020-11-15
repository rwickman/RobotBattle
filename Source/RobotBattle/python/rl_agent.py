from agent import Agent
import numpy as np
import tensorflow as tf
from tensorflow.python.ops import gradients
import time
from actor_critic import ppo_loss_discrete

class RLAgent(Agent):
    def __init__(self, args, agent_client, agent_train_data, global_model):
        super().__init__(args, agent_client, agent_train_data)
        self._global_model = global_model
        self._local_model = global_model.clone()
        
    def start_training(self):
        # Connect to remote agent
        self._agent_client.connect()

        # Start running episodes
        for i in range(self._args.episodes):
            ep_dict = self._run_episode()
            print("Done running episode")
            self._train_model(ep_dict)
            self._reset()

    def _run_episode(self):
        # The critic value predictions
        val_preds = []

        # The actor action softmax predictions
        actor_preds = []
        
        # The one-hot encoding of edges that were pruned
        actions = [] 

        # The episode states
        states = []

        # The episode rewards
        rewards = []

        # Get first episode state
        state_dict = self._agent_client.recv_state()
        while True:   
            # Parse state to transform to model input form
            state_input = self._parse_state(state_dict)

            # Get the agent action
            actor_pred, critic_pred, action = self._get_action_value(state_input)

            # Tell client to have remote agent perform the action
            self._agent_client.send_action(action)

            # Save timestep information
            val_preds.append(critic_pred)
            actor_preds.append(actor_pred)
            states.append(state_input)

            # Create one-hot encoded action vector
            action_one_hot = np.zeros(self._args.action_space)
            #print("action: ", action)
            action_one_hot[action] = 1
            actions.append(action_one_hot)
            
            # Get next state
            state_dict = self._agent_client.recv_state()
            
            # Get reward for previous action
            rewards.append(self._get_episode_reward(state_dict))
            
            # Check if episode is over (i.e., in terminal state)
            if self._is_episode_terminated(state_dict):
                print(actor_pred)
                break
                
        
        return {
            "val_preds" : np.array(val_preds),
            "actor_preds" : np.array(actor_preds),
            "states" : np.array(states),
            "actions" : np.array(actions),
            "rewards" : np.array(rewards)
        }

    def _get_action_value(self, state_input):
        # Get predictions from ActorCritic model
        actor_pred, critic_pred = self._local_model(state_input)
        #critic_pred = critic_pred[0,0]
        action_distr = actor_pred.numpy()[0]

        # Sample from softmax action to get action
        action = np.random.choice(len(action_distr), p=action_distr)
        
        return actor_pred, critic_pred, action
    
    def _reset(self):
        # Reset the environment
        self._local_model.reset_states()
        self._agent_client.send_reset()

    def _train_model(self, ep_dict):
        print("Adding adv and return")
        self._add_ret_and_adv(ep_dict)
        
        total_actor_loss = 0
        total_critic_loss = 0
        ep_len = ep_dict["states"].shape[0]
        for _ in range(self._args.epochs):
            self._local_model.reset_states()
            for t in range(ep_len):
                with tf.GradientTape() as tape:  
                    actor_pred, critic_pred = self._local_model(ep_dict["states"][t])
                    actor_loss, critic_loss = ppo_loss_discrete(
                        ep_dict["adv"][t],
                        ep_dict["actor_preds"][t],
                        ep_dict["actions"][t],
                        actor_pred,
                        critic_pred,
                        ep_dict["returns"][t])
                    total_loss = actor_loss + critic_loss
                    #print("Total loss: ", total_loss)

                # Compute the gradients based on the local model
                grads = tape.gradient(total_loss, self._local_model.trainable_variables)
                if t == 0:
                    sum_grad = grads
                    for i, g in enumerate(grads):
                        sum_grad[i] = tf.convert_to_tensor(g) 
                else:
                    for i, g in enumerate(grads):
                        sum_grad[i] += tf.convert_to_tensor(g)
                
                total_actor_loss += actor_loss.numpy()
                total_critic_loss += critic_loss.numpy()

                
            # Average the grads
            for i in range(len(sum_grad)):
                sum_grad[i] = sum_grad[i] / ep_len
            
            # Apply the gradients to global model and update local model weights
            self._local_model.set_weights(self._global_model.apply_gradients(sum_grad))

        # Compute average loss
        avg_actor_loss = total_actor_loss / (ep_len * self._args.epochs)
        avg_critic_loss = total_critic_loss / (ep_len * self._args.epochs)
        print("avg_actor_loss: ", avg_actor_loss)
        print("avg_critic_loss: ", avg_critic_loss)
        self._global_model.save_rl_episode(
            avg_actor_loss,
            avg_critic_loss,
            ep_dict["avg episode return"])
        
        self._agent_train_data.save()