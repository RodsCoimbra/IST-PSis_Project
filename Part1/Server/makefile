all: game-server

game-server: game-server.o FIFOUtils.o communication.o space_ship.o Utils.o space_aliens.o
	gcc game-server.o FIFOUtils.o communication.o space_ship.o Utils.o space_aliens.o -o game-server -lzmq -lncurses

game-server.o: game-server.c game-server.h ../remote-char.h FIFOUtils.h communication.h space_ship.h Utils.h space_aliens.h
	gcc -g -c game-server.c -o game-server.o

FIFOUtils.o: FIFOUtils.c FIFOUtils.h
	gcc -g -c FIFOUtils.c -o FIFOUtils.o

communication.o: communication.c communication.h
	gcc -g -c communication.c -o communication.o

space_ship.o: space_ship.c space_ship.h ../remote-char.h
	gcc -g -c space_ship.c -o space_ship.o

Utils.o: Utils.c Utils.h ../remote-char.h
	gcc -g -c Utils.c -o Utils.o

space_aliens.o: space_aliens.c space_aliens.h ../remote-char.h Utils.h
	gcc -g -c space_aliens.c -o space_aliens.o

clean:
	rm -f *.o game-server