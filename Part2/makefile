CFLAGS = -g -Wall

# Directories
SERVER          = Server
UTILS           = Utils
DISP_CLIENT  	= Display-client

#Compiled
SERVER_C 		= $(SERVER)/Compiled
UTILS_C 		= $(UTILS)/Compiled
DISP_CLIENT_C 	= $(DISP_CLIENT)/Compiled

all: game-server display-client outer-space-display astronaut-client

#Game Server
game-server: game-server.o communication.o space_ship.o Utils.o space_aliens.o space.pb-c.c
	gcc $(CFLAGS) space.pb-c.c $(SERVER_C)/game-server.o $(UTILS_C)/communication.o $(SERVER_C)/space_ship.o $(UTILS_C)/Utils.o $(SERVER_C)/space_aliens.o -o game-server -lzmq -lncurses -lprotobuf-c -lpthread

#Display Client
display-client: astronaut-display-client.o communication.o Utils.o outer-space-display.o space.pb-c.c astronaut-client.o
	gcc $(CFLAGS) space.pb-c.c $(DISP_CLIENT_C)/astronaut-display-client.o $(UTILS_C)/communication.o $(UTILS_C)/Utils.o $(DISP_CLIENT_C)/outer-space-display.o $(DISP_CLIENT_C)/astronaut-client.o -o astronaut-display-client -lzmq -lncurses -lprotobuf-c -lpthread

#Outer Space Display
outer-space-display: display.o communication.o Utils.o outer-space-display.o
	gcc $(CFLAGS) space.pb-c.c $(DISP_CLIENT_C)/outer-space-display.o $(DISP_CLIENT_C)/display.o $(UTILS_C)/communication.o $(UTILS_C)/Utils.o -o outer-space-display -lzmq -lncurses -lpthread -lprotobuf-c

#Astronaut Client
astronaut-client: client.o communication.o Utils.o astronaut-client.o
	gcc $(CFLAGS) space.pb-c.c $(DISP_CLIENT_C)/client.o $(UTILS_C)/communication.o $(UTILS_C)/Utils.o $(DISP_CLIENT_C)/astronaut-client.o -o astronaut-client -lzmq -lncurses -lpthread -lprotobuf-c


#Protobuf
proto: space.proto
	protoc-c --python_out=. space.proto
	protoc-c --c_out=. space.proto

#Compile
client.o: $(DISP_CLIENT)/client.c $(DISP_CLIENT)/client.h $(UTILS)/remote-char.h $(UTILS)/communication.h $(UTILS)/Utils.h
	gcc $(CFLAGS) -c $(DISP_CLIENT)/client.c -o $(DISP_CLIENT)/Compiled/client.o

display.o: $(DISP_CLIENT)/display.c $(DISP_CLIENT)/astronaut-display-client.h $(UTILS)/remote-char.h $(UTILS)/communication.h $(UTILS)/Utils.h
	gcc $(CFLAGS) -c $(DISP_CLIENT)/display.c -o $(DISP_CLIENT)/Compiled/display.o

astronaut-client.o: $(DISP_CLIENT)/astronaut-client.c $(DISP_CLIENT)/astronaut-client.h $(UTILS)/remote-char.h $(UTILS)/communication.h $(UTILS)/Utils.h $(DISP_CLIENT)/client.h
	gcc $(CFLAGS) -c $(DISP_CLIENT)/astronaut-client.c -o $(DISP_CLIENT)/Compiled/astronaut-client.o

astronaut-display-client.o: $(DISP_CLIENT)/astronaut-display-client.c $(DISP_CLIENT)/astronaut-display-client.h $(UTILS)/remote-char.h $(UTILS)/communication.h $(UTILS)/Utils.h
	gcc $(CFLAGS) -c $(DISP_CLIENT)/astronaut-display-client.c -o $(DISP_CLIENT)/Compiled/astronaut-display-client.o

outer-space-display.o: $(DISP_CLIENT)/outer-space-display.c $(DISP_CLIENT)/outer-space-display.h $(UTILS)/remote-char.h
	gcc $(CFLAGS) -c $(DISP_CLIENT)/outer-space-display.c -o $(DISP_CLIENT)/Compiled/outer-space-display.o

game-server.o: $(SERVER)/game-server.c $(SERVER)/game-server.h $(UTILS)/remote-char.h $(UTILS)/communication.h $(SERVER)/space_ship.h $(UTILS)/Utils.h $(SERVER)/space_aliens.h
	gcc $(CFLAGS) -c $(SERVER)/game-server.c -o $(SERVER)/Compiled/game-server.o

space_aliens.o: $(SERVER)/space_aliens.c $(SERVER)/space_aliens.h $(UTILS)/remote-char.h $(UTILS)/Utils.h
	gcc $(CFLAGS) -c $(SERVER)/space_aliens.c -o $(SERVER)/Compiled/space_aliens.o

space_ship.o: $(SERVER)/space_ship.c $(SERVER)/space_ship.h $(UTILS)/remote-char.h $(UTILS)/Utils.h
	gcc $(CFLAGS) -c $(SERVER)/space_ship.c -o $(SERVER)/Compiled/space_ship.o -lpthread

communication.o: $(UTILS)/communication.c $(UTILS)/communication.h $(UTILS)/remote-char.h
	gcc $(CFLAGS) -c $(UTILS)/communication.c -o $(UTILS)/Compiled/communication.o -lprotobuf-c

Utils.o: $(UTILS)/Utils.c $(UTILS)/Utils.h $(UTILS)/remote-char.h 
	gcc $(CFLAGS) -c $(UTILS)/Utils.c -o $(UTILS)/Compiled/Utils.o -lpthread

clean:
	rm -f */Compiled/*.o game-server astronaut-display-client outer-space-display astronaut-client