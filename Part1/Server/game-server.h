#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "../remote-char.h"
#include "communication.h"
#include "Utils.h"
#include "space_ship_utils.h"
#include "space_aliens_utils.h"
#include "FIFOUtils.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <time.h>

void run_aliens(void *responder, void *publisher, WINDOW *space);

void run_players(void *responder, void *publisher, WINDOW *space, WINDOW *score_board);

void run_game(void *responder, void *publisher);

void astronaut_connect(ship_info_t ship_data[], remote_char_t m, WINDOW *space, WINDOW *score_board);

#endif