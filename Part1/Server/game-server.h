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
#include "../Utils/FIFOUtils.h"

void run_aliens(void *, void *, WINDOW *);

void run_players(void *, void *, WINDOW *, WINDOW *);

void run_game(void *, void *);

void astronaut_connect(ship_info_t[], remote_char_t *, WINDOW *, WINDOW *);

#endif