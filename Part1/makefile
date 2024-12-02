all: game-server astronaut-client

game-server: Server/game-server.o Server/FIFOUtils.o Server/communication.o Server/space_ship_utils.o Server/Utils.o Server/space_aliens_utils.o
	gcc Server/game-server.o Server/FIFOUtils.o Server/communication.o Server/space_ship_utils.o Server/Utils.o Server/space_aliens_utils.o -o game-server -lzmq -lncurses

Server/game-server.o: Server/game-server.c Server/game-server.h remote-char.h Server/FIFOUtils.h Server/communication.h Server/space_ship_utils.h Server/Utils.h Server/space_aliens_utils.h
	gcc -c Server/game-server.c -o Server/game-server.o

astronaut-client: Client/astronaut-client.o
	gcc Client/astronaut-client.o -o astronaut-client -lzmq -lncurses

Client/astronaut-client.o: Client/astronaut-client.c Client/astronaut-client.h remote-char.h
	gcc -c Client/astronaut-client.c -o Client/astronaut-client.o

Server/FIFOUtils.o: Server/FIFOUtils.c Server/FIFOUtils.h
	gcc -g -c Server/FIFOUtils.c -o Server/FIFOUtils.o

Server/communication.o: Server/communication.c Server/communication.h
	gcc -g -c Server/communication.c -o Server/communication.o

Server/space_ship_utils.o: Server/space_ship_utils.c Server/space_ship_utils.h remote-char.h
	gcc -g -c Server/space_ship_utils.c -o Server/space_ship_utils.o

Server/Utils.o: Server/Utils.c Server/Utils.h remote-char.h
	gcc -g -c Server/Utils.c -o Server/Utils.o

Server/space_aliens_utils.o: Server/space_aliens_utils.c Server/space_aliens_utils.h remote-char.h Server/Utils.h
	gcc -g -c Server/space_aliens_utils.c -o Server/space_aliens_utils.o

clean:
	rm -f *.o Client/*.o Server/*.o game-server astronaut-client