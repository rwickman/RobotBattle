import numpy as np
from agent_train_data import LocationID

from actor_critic import ActorCritic

class Agent:
    def __init__(self, args, agent_train_data):
        self._args = args
        self._agent_train_data = agent_train_data
        self.model_weights_dir = "model/actor_critic"

    def _parse_state(self, state_dict):
        state_input = []
        self._agent_train_data.update_stats(state_dict)
        player_state = state_dict["StateRewardPair"]["State"]["PlayerTransform"]
        hit_results = state_dict["StateRewardPair"]["State"]["HitResults"] + [player_state]
        for hit in hit_results:
            hit_result = []
            hit_result.append(hit["Health"] / 5)
            hit_result.append(hit["IsAttacking"])
            hit_result.append(self._agent_train_data.norm_loc(hit["Location"][0], LocationID.X))
            hit_result.append(self._agent_train_data.norm_loc(hit["Location"][1], LocationID.Y))
            hit_result.append(self._agent_train_data.norm_loc(hit["Location"][2], LocationID.Z))
            hit_result.append(self._agent_train_data.norm_rot(hit["Rotation"][0]) )
            hit_result.append(self._agent_train_data.norm_rot(hit["Rotation"][1]))
            hit_result.append(self._agent_train_data.norm_rot(hit["Rotation"][2]))
            hit_result.append(hit["Type"])
            state_input.append(hit_result)

        return np.array([state_input], dtype=np.float32)
    
    def _is_episode_terminated(self, state_dict):
        return state_dict["StateRewardPair"]["IsEpisodeTerminated"]
    
    def _get_episode_reward(self, state_dict):
        return state_dict["StateRewardPair"]["Reward"]
    

    def _add_ret_and_adv(self, ep_dict):
        """Add return and advantage to ep_dict."""
        ep_len = ep_dict["rewards"].shape[0]
        ep_dict["adv"] = np.zeros(ep_dict["rewards"].shape, dtype=np.float32)
        ep_dict["returns"] = np.empty(ep_dict["rewards"].shape, dtype=np.float32)
        
        # Create the returns for every timestep in the episode
        for t in range(ep_len-1, -1, -1):
            ep_dict["returns"][t] = ep_dict["rewards"][t]

            if t < ep_len - 1:
                ep_dict["returns"][t] += ep_dict["returns"][t+1] * self._args.gamma
        # Normalize the returns
        ep_dict["returns"] = ep_dict["returns"]/110.0
        
        # if "val_preds" in ep_dict:

        ep_dict["adv"] = ep_dict["returns"] - ep_dict["val_preds"][:, 0, 0]

        



        ep_dict["avg episode return"] = np.mean(ep_dict["returns"])


    def _clone_global_model(self):
        """Clone the global model."""
        cloned_model = ActorCritic(self._args)
        if self._args.load_model:
            cloned_model.load_weights(self.model_weights_dir)
        #cloned_model.set_weights(self._actor_critic.get_weights())
        return cloned_model