import argparse, threading
from multiprocessing import Process, Queue, Lock
import numpy as np

MAX_AGENTS = 4
# from actor_critic import ActorCritic
from global_model import GlobalModel
from rl_agent import RLAgent

from agent_train_data import AgentTrainData

def main(args):
    if args.num_agents > MAX_AGENTS:
        args.num_agents = MAX_AGENTS

    global_model = GlobalModel(args)
    train_queue = Queue(maxsize=1)
    grad_lock = Lock()
    agents = []
    for i in range(args.num_agents):
        #agent_client = AgentClient(args)
        agent_train_data = AgentTrainData(args, i)
        rl_agent = RLAgent(args, agent_train_data, global_model)
        #agent_worker = threading.Thread(target=rl_agent.start_training)
        agent_worker = Process(target=rl_agent.start_training, args=(grad_lock, train_queue))
        #agent_worker = Process(target=RLAgent, args=(args, agent_train_data, global_model))
        agent_worker.start()
        print("Called Start")
        agents.append(agent_worker)

    for i in range(args.episodes * args.num_agents):
        train_list = train_queue.get()
        global_weights = global_model.apply_gradients(train_list[0])
        global_model.save_rl_episode(
            train_list[1],
            train_list[2],
            train_list[3]
        )
        train_queue.put(global_weights)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--epochs", type=int, default=1,
            help="Number of epochs to train on each episode.")
    parser.add_argument("--num_agents", type=int, default=2,
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
    