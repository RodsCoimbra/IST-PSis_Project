all: game-server astronaut-client outer-space-display

game-server: game-server.o communication.o space_ship.o Utils.o space_aliens.o
	gcc Server/Compiled/game-server.o Utils/Compiled/communication.o Server/Compiled/space_ship.o Utils/Compiled/Utils.o Server/Compiled/space_aliens.o -o game-server -lzmq -lncurses

game-server.o: Server/game-server.c Server/game-server.h Utils/remote-char.h Utils/communication.h Server/space_ship.h Utils/Utils.h Server/space_aliens.h
	gcc -g -c Server/game-server.c -o Server/Compiled/game-server.o

astronaut-client: Client/astronaut-client.o communication.o 
	gcc Client/Compiled/astronaut-client.o Utils/Compiled/communication.o -o astronaut-client -lzmq -lncurses

Client/astronaut-client.o: Client/astronaut-client.c Client/astronaut-client.h Utils/remote-char.h
	gcc -g -c Client/astronaut-client.c -o Client/Compiled/astronaut-client.o

outer-space-display: Remote-Display-Client/outer-space-display.o communication.o space_ship.o space_aliens.o Utils.o
	gcc Remote-Display-Client/Compiled/outer-space-display.o Utils/Compiled/communication.o Server/Compiled/space_ship.o Server/Compiled/space_aliens.o Utils/Compiled/Utils.o -o outer-space-display -lzmq -lncurses

Remote-Display-Client/outer-space-display.o: Remote-Display-Client/outer-space-display.c Remote-Display-Client/outer-space-display.h Utils/remote-char.h
	gcc -g -c Remote-Display-Client/outer-space-display.c -o Remote-Display-Client/Compiled/outer-space-display.o

communication.o: Utils/communication.c Utils/communication.h
	gcc -g -c Utils/communication.c -o Utils/Compiled/communication.o

space_ship.o: Server/space_ship.c Server/space_ship.h Utils/remote-char.h
	gcc -g -c Server/space_ship.c -o Server/Compiled/space_ship.o

Utils.o: Utils/Utils.c Utils/Utils.h Utils/remote-char.h
	gcc -g -c Utils/Utils.c -o Utils/Compiled/Utils.o

space_aliens.o: Server/space_aliens.c Server/space_aliens.h Utils/remote-char.h Utils/Utils.h
	gcc -g -c Server/space_aliens.c -o Server/Compiled/space_aliens.o

clean:
	rm -f */Compiled/*.o game-server astronaut-client outer-space-display