import socket
import json, time
HOST = "localhost"
PORT = 12001
HEADER_LENGTH = 4
MAX_BODY_LENGTH =  4096
ACTION_SIZE = 10

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("CONNECTED")
    while True:
        body_length = s.recv(HEADER_LENGTH)
        print("body_length", body_length)
        data = s.recv(int(body_length.decode()))
        print("READ DATA")
        print(len(data))
        print(data)
        state_reward_pair = json.loads(data.decode())
        if not state_reward_pair["StateRewardPair"]["IsEpisodeTerminated"]:

            #print("state_reward_pair", state_reward_pair)
            #print(data.decode())
            action = "{:4d}{}".format(ACTION_SIZE, "0000010010")
            print(action)
            print(len(action))
            s.sendall(action.encode())
        else:
            time.sleep(5)
            s.sendall(b"  OK")

