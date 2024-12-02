all: game-server\

game-server: game-server.o FIFOUtils.o
	gcc game-server.o FIFOUtils.o -o game-server -lzmq -lncurses

game-server.o: game-server.c game-server.h ../remote-char.h FIFOUtils.h
	gcc -g -c game-server.c -o game-server.o

FIFOUtils.o: FIFOUtils.c FIFOUtils.h
	gcc -g -c FIFOUtils.c -o FIFOUtils.o

clean:
	rm -f *.o *.o game-server