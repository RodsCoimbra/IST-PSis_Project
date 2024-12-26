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
    all_ships_t* data;
    WINDOW* space;
    void *publisher;
    int *game_end;  
} run_alien_args;

typedef struct
{
    WINDOW* space;
    WINDOW* score_board;
    WINDOW* numbers;
    int *game_end;
} display_args;


void *run_aliens(void *);

void run_players(all_ships_t , WINDOW *, WINDOW *, void *, void *, WINDOW*, int *);

void *keyboard_handler(void *arg);

void run_game();

void *thread_refresh(void *args);

#endif