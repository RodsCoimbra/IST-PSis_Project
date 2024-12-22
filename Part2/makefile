CFLAGS = -O3 -Wall
all: game-server astronaut-client outer-space-display astronaut-display-client

game-server: game-server.o communication.o space_ship.o Utils.o space_aliens.o
	gcc $(CFLAGS) Server/Compiled/game-server.o Utils/Compiled/communication.o Server/Compiled/space_ship.o Utils/Compiled/Utils.o Server/Compiled/space_aliens.o -o game-server -lzmq -lncurses -lpthread

game-server.o: Server/game-server.c Server/game-server.h Utils/remote-char.h Utils/communication.h Server/space_ship.h Utils/Utils.h Server/space_aliens.h
	gcc $(CFLAGS) -c Server/game-server.c -o Server/Compiled/game-server.o

astronaut-client: astronaut-client.o communication.o Utils.o
	gcc $(CFLAGS) Client/Compiled/astronaut-client.o Utils/Compiled/communication.o Utils/Compiled/Utils.o -o astronaut-client -lzmq -lncurses

astronaut-client.o: Client/astronaut-client.c Client/astronaut-client.h Utils/remote-char.h
	gcc $(CFLAGS) -c Client/astronaut-client.c -o Client/Compiled/astronaut-client.o

outer-space-display: outer-space-display.o communication.o space_ship.o space_aliens.o Utils.o
	gcc $(CFLAGS) Remote-Display-Client/Compiled/outer-space-display.o Utils/Compiled/communication.o Server/Compiled/space_ship.o Server/Compiled/space_aliens.o Utils/Compiled/Utils.o -o outer-space-display -lzmq -lncurses

outer-space-display.o: Remote-Display-Client/outer-space-display.c Remote-Display-Client/outer-space-display.h Utils/remote-char.h
	gcc $(CFLAGS) -c Remote-Display-Client/outer-space-display.c -o Remote-Display-Client/Compiled/outer-space-display.o

astronaut-display-client: astronaut-display-client.o communication.o Utils.o space_ship.o astronaut-client.o outer-space-display.o
	gcc $(CFLAGS) Display-client/Compiled/astronaut-display-client.o Utils/Compiled/communication.o Utils/Compiled/Utils.o Server/Compiled/space_ship.o Client/Compiled/astronaut-client.o Remote-Display-Client/Compiled/outer-space-display.o -o astronaut-display-client -lzmq -lncurses -lpthread

astronaut-display-client.o: Display-client/astronaut-display-client.c Display-client/astronaut-display-client.h Utils/remote-char.h Server/space_ship.h Utils/Utils.h Utils/communication.h Client/astronaut-client.h Remote-Display-Client/outer-space-display.h
	gcc $(CFLAGS) -c Display-client/astronaut-display-client.c -o Display-client/Compiled/astronaut-display-client.o

communication.o: Utils/communication.c Utils/communication.h
	gcc $(CFLAGS) -c Utils/communication.c -o Utils/Compiled/communication.o

space_ship.o: Server/space_ship.c Server/space_ship.h Utils/remote-char.h
	gcc $(CFLAGS) -c Server/space_ship.c -o Server/Compiled/space_ship.o

Utils.o: Utils/Utils.c Utils/Utils.h Utils/remote-char.h
	gcc $(CFLAGS) -c Utils/Utils.c -o Utils/Compiled/Utils.o

space_aliens.o: Server/space_aliens.c Server/space_aliens.h Utils/remote-char.h Utils/Utils.h
	gcc $(CFLAGS) -c Server/space_aliens.c -o Server/Compiled/space_aliens.o

clean:
	rm -f */Compiled/*.o game-server astronaut-client outer-space-display astronaut-display-client