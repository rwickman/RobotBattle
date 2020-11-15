import argparse, threading
import numpy as np

NUM_AGENTS = 4

# from actor_critic import ActorCritic
from global_model import GlobalModel
from rl_agent import RLAgent
from client import AgentClient
from agent_train_data import AgentTrainData

def main(args):
    global_model = GlobalModel(args)

    agents = []
    for i in range(NUM_AGENTS):
        agent_client = AgentClient(args)
        agent_train_data = AgentTrainData(args, i)
        rl_agent = RLAgent(args, agent_client, agent_train_data, global_model)
        agent_worker = threading.Thread(target=rl_agent.start_training)
        agent_worker.start()
        agents.append(agent_worker)

    # agent_client = AgentClient(args)
    # agent_train_data = AgentTrainData(args)
    # rl_agent = RLAgent(args, agent_client, agent_train_data, global_model)
    # rl_agent.start_training()

    #actor_critic_model = ActorCritic(args)





if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--epochs", type=int, default=2,
            help="Number of epochs to train on each episode.")
    parser.add_argument("--episodes", type=int, default=100000,
            help="Number of episodes to train on.")
    parser.add_argument("--lr", type=float, default=1e-5,
            help="The learning rate.")
    parser.add_argument("--gamma", type=float, default=0.99,
            help="The learning rate.")
    parser.add_argument("--load_model", action="store_true",
        help="Load the saved models.")
    parser.add_argument("--num_types", type=int, default=6,
            help="Number of object types.")
    parser.add_argument("--type_emb", type=int, default=32,
            help="The type embedding size.")
    parser.add_argument("--obj_emb", type=int, default=32,
            help="The object embedding size.")
    parser.add_argument("--lstm_units", type=int, default=128,
            help="The object embedding size.")
    parser.add_argument("--dense_units", type=int, default=128,
        help="The object embedding size.")
    parser.add_argument("--dropout_rate", type=float, default=0.1,
        help="Dropout rate")
    parser.add_argument("--port", type=int, default=12001,
        help="Port number")
    parser.add_argument("--hostname", type=str, default="localhost",
        help="Hostname")
    parser.add_argument("--header_len", type=int, default=4,
        help="Packet header length")
    parser.add_argument("--action_space", type=int, default=1024,
        help="The total size of the action space.")
    
    main(parser.parse_args())
    