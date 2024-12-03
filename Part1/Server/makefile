all: game-server

game-server: 
	gcc Compiled/game-server.o ../Utils/Compiled/FIFOUtils.o ../Utils/Compiled/communication.o Compiled/space_ship.o ../Utils/Compiled/Utils.o Compiled/space_aliens.o -o game-server -lzmq -lncurses


