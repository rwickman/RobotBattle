import json, os
from enum import Enum

MAX_ROTATION = 360
MIN_ROTATION = -360

class LocationID(Enum):
    X = 0,
    Y=  1,
    Z = 2


class AgentTrainData:
    """Keeps track of various important train data import for one instance of an agent."""
    
    def __init__(self, args, agent_data_json="model/agent_0_data.json"):
        self._args = args
        self._agent_data_json = agent_data_json
        self._setup()

    def _setup(self):
        if not self.load():
            self._train_data = {
                "rl_episodes" : 0,
                "max_loc_x" : 1,
                "min_loc_x" : 0,
                "max_loc_y" : 1,
                "min_loc_y" : 0,
                "max_loc_z" : 1,
                "min_loc_z" : 0
            }        
    
    def load(self):
        """Load train data.

        Returns:
            boolean indicating if it successfully loaded.
        """
        if self._args.load_model and os.path.exists(self._agent_data_json ): 
            with open(self._agent_data_json , "r") as f:
                self._train_data = json.load(f)
            return True
        else:
            return False

    def save(self):
        """Save the data as json"""
        # Increment number of episodes
        self._train_data["rl_episodes"] += 1

        with open(self._agent_data_json , "w") as f:
            json.dump(self._train_data, f)

    def update_stats(self, state_dict):
        """Update state statistics.
        
        Args:
            state_dict: the json state sent over the network.
        """
        player_state = state_dict["StateRewardPair"]["State"]["PlayerTransform"]
        hit_results = state_dict["StateRewardPair"]["State"]["HitResults"] + [player_state]
        for hit in hit_results:
            self._train_data["min_loc_x"] = min(self._train_data["min_loc_x"], hit["Location"][0])
            self._train_data["max_loc_x"] = max(self._train_data["max_loc_x"], hit["Location"][0])
            self._train_data["min_loc_y"] = min(self._train_data["min_loc_y"], hit["Location"][1])
            self._train_data["max_loc_y"] = max(self._train_data["max_loc_y"], hit["Location"][1])
            self._train_data["min_loc_z"] = min(self._train_data["min_loc_z"], hit["Location"][2])
            self._train_data["max_loc_z"] = max(self._train_data["max_loc_z"], hit["Location"][2])
    
    def norm_rot(self, rot):
        """Normalize rotation."""
        return (rot - MIN_ROTATION) / (MAX_ROTATION - MIN_ROTATION)
    
    def norm_loc(self, loc, loc_id):
        """Normalize location."""
        if loc_id == LocationID.X:
            return (loc - self._train_data["max_loc_x"]) / (self._train_data["max_loc_x"] - self._train_data["min_loc_x"])   
        elif loc_id == LocationID.Y:
            return (loc - self._train_data["max_loc_x"]) / (self._train_data["max_loc_y"] - self._train_data["min_loc_y"])
        else:
            return (loc - self._train_data["max_loc_z"]) / (self._train_data["max_loc_z"] - self._train_data["min_loc_z"])