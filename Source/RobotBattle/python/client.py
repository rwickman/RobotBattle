import socket
import json, time
HOST = "localhost"
PORT = 12001
HEADER_LENGTH = 4
MAX_BODY_LENGTH =  4096
ACTION_SIZE = 10

class AgentClient:
    def __init__(self, args):
        self._args = args
    
    def __del__(self):
        self._socket.close()
    
    def connect(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect((self._args.hostname, self._args.port))
        print("CONNECTED")

    def recv_state(self):
        state_body_len = self._socket.recv(self._args.header_len)
        state_body = self._socket.recv(int(state_body_len.decode()))
        state_reward_pair = json.loads(state_body.decode())
        print(state_reward_pair)
        return state_reward_pair 
                
                #     action = "{:4d}{}".format(ACTION_SIZE, "0000010010")
                #     print(action)
                #     print(len(action))
                #     s.sendall(action.encode())
                # else:
                #     time.sleep(5)
                #     print("SENDING RESTART SIGNAL")
                #     s.sendall(b"  OK")
    def send_action(self, action):
         # Encode action as bit string with header size
        action_packet = "{0:4d}{1:010b}".format(10, action)
        #action_packet ="{0:4d}{1}".format(10, "0100100100")
        print(action_packet)
        # Send action to remote agent
        self._socket.sendall(action_packet.encode())

    def send_reset(self):
        """Send signal to reset the environment."""
        self._socket.sendall(b"  OK")