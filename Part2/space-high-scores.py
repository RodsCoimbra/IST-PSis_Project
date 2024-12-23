# This code was done with the help of ChatGPT
import zmq
import space_pb2

def main():
    context = zmq.Context()
    subscriber = context.socket(zmq.SUB)
    subscriber.connect("tcp://localhost:5506")
    subscriber.setsockopt_string(zmq.SUBSCRIBE, "Score")  # Topic

    scoreboard = {}

    while True:
        # Read the topic
        topic = subscriber.recv_string()
        if topic == "Score":
            # Read the protobuf message
            msg = subscriber.recv()
            scoreboard = space_pb2.ScoreBoard()
            scoreboard.ParseFromString(msg)

            print("\033c", end="")  # clear screen
            print("=== Current Astronaut Scores ===")
            for entry in scoreboard.scores:
                print(f"{chr(entry.astronaut)} : {entry.points}")


if __name__ == "__main__":
    main()