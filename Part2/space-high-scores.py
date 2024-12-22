import zmq
import space_pb2
import time

def main():
    context = zmq.Context()
    subscriber = context.socket(zmq.SUB)
    subscriber.connect("tcp://localhost:5506")
    subscriber.setsockopt_string(zmq.SUBSCRIBE, "Display")  # Topic

    scoreboard = {}

    while True:
        # First frame is the topic
        topic = subscriber.recv_string()
        # Second frame is the protobuf data
        data = subscriber.recv()

        update = space_pb2.ScoreUpdate()
        update.ParseFromString(data)

        # Update local scoreboard
        changed = False
        old_score = scoreboard.get(update.astronaut, -1)
        if update.points != old_score:
            scoreboard[update.astronaut] = update.points
            changed = True

        # Display scores if changed
        if changed:
            print("\033c", end="")  # Clear screen
            print("=== Current Astronaut Scores ===")
            for astro, pts in scoreboard.items():
                print(f"{astro} : {pts}")

        time.sleep(0.1)

if __name__ == "__main__":
    main()