all: game-server astronaut-client

game-server: Server/game-server.o Server/FIFOUtils.o
	gcc Server/game-server.o Server/FIFOUtils.o -o game-server -lzmq -lncurses

Server/game-server.o: Server/game-server.c Server/game-server.h remote-char.h Server/FIFOUtils.h
	gcc -c Server/game-server.c -o Server/game-server.o

astronaut-client: Client/astronaut-client.o
	gcc Client/astronaut-client.o -o astronaut-client -lzmq -lncurses

Client/astronaut-client.o: Client/astronaut-client.c Client/astronaut-client.h remote-char.h
	gcc -c Client/astronaut-client.c -o Client/astronaut-client.o

Server/FIFOUtils.o: Server/FIFOUtils.c Server/FIFOUtils.h
	gcc -g -c Server/FIFOUtils.c -o Server/FIFOUtils.o

clean:
	rm -f *.o Client/*.o Server/*.o game-server astronaut-client