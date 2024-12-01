game-server: game-server.o
	gcc game-server.o -o game-server -lzmq -lncurses

game-server.o: game-server.c remote-char.h game-server.h
	gcc -g -c game-server.c -o game-server.o

clean:
	rm -f *.o game-server