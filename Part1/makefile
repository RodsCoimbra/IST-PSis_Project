all: game-server astronaut-client

game-server: Server/game-server.o
	gcc Server/game-server.o -o game-server -lzmq -lncurses

game-server.o: game-server.c remote-char.h game-server.h
	gcc -c Server/game-server.c -o Server/game-server.o

astronaut-client: Client/astronaut-client.o
	gcc Client/astronaut-client.o -o astronaut-client -lzmq -lncurses

astronaut-client.o: astronaut-client.c remote-char.h astronaut-client.h
	gcc -c Client/astronaut-client.c -o Client/astronaut-client.o

clean:
	rm -f *.o Client/*.o Server/*.o game-server astronaut-client