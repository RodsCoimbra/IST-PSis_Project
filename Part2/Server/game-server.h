#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "../Utils/Utils.h"
#include "space_ship.h"
#include "space_aliens.h"

typedef struct
{
    alien_info_t* aliens;
    WINDOW* space;
} run_alien_args;


void *run_aliens(void *);

void run_players(all_ships_t, WINDOW *, WINDOW *);

void run_game();

#endif