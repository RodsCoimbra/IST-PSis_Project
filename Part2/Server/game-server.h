#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <time.h>
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "../Utils/Utils.h"
#include "space_ship.h"
#include "space_aliens.h"

void run_aliens(int encryption);

void run_players(int encryption);

void run_game();

#endif