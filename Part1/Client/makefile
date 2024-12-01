astronaut-client: astronaut-client.o
	gcc astronaut-client.o -o astronaut-client -lzmq -lncurses

astronaut-client.o: astronaut-client.c remote-char.h astronaut-client.h
	gcc -g -c astronaut-client.c -o astronaut-client.o

clean:
	rm -f *.o astronaut-client