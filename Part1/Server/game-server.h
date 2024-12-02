#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <ncurses.h>
#include "../remote-char.h"
#include "FIFOUtils.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <time.h>

#define WINDOW_SIZE 20
#define N_SHIPS 8
#define N_ALIENS ((WINDOW_SIZE - 2) * (WINDOW_SIZE - 2) / 3)
#define MAX_POS WINDOW_SIZE - 4
#define MIN_POS 3
#define MID_POS WINDOW_SIZE / 2
#define RECHARGING_TIME 3
#define STUN_TIME 10

typedef enum status_t
{
    RECHARGING,
    STUNNED
} status_t;

typedef struct position_info_t
{
    int x, y;
} position_info_t;

typedef struct alien_info_t
{
    position_info_t position;
    int alive;
} alien_info_t;

typedef struct ship_info_t
{
    int ship;
    position_info_t position;
    int points;
    movement_t move_type;
    time_t timeouts[2];
} ship_info_t;

// direction_t random_direction()
// {
//     return random() % 4;
// }

void run_aliens(void *responder, void *publisher, WINDOW *space);

void run_players(void *responder, void *publisher, WINDOW *space, WINDOW *score_board);

void run_game(void *responder, void *publisher);

void initialize_connection(void **context, void **responder, void **publisher);

void initialize_ship(ship_info_t *ship_data, position_info_t spawn_point, char ship);

ship_info_t *find_ship_info(ship_info_t ship_data[], int ship);

void initialize_ncurses();

void initialize_window(WINDOW **space, WINDOW **score_board);

int create_new_ship(ship_info_t ship_data[]);

void update_score_board(WINDOW **score_board, ship_info_t ship_data[]);

void initialize_aliens(alien_info_t alien_data[]);

void alien_movement(alien_info_t alien_data[]);

void new_position(ship_info_t *current_ship, direction_t direction);

void clip_value(int *value, int min, int max);

void update_window_char(WINDOW *space, position_info_t update_position, char c);

void send_msg(void *responder, remote_char_t *m);

void recv_msg(void *responder, remote_char_t *m);

void publish_display_data(void *publisher, ship_info_t *ship_data, char *topic);

#endif