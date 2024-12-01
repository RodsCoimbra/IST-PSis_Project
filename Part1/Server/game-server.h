#include <ncurses.h>
#include "remote-char.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>

#define WINDOW_SIZE 20
#define N_SHIPS 8
#define N_ALIENS ((WINDOW_SIZE - 2) * (WINDOW_SIZE - 2) / 3)
#define MAX_POS WINDOW_SIZE - 4
#define MIN_POS 3
#define MID_POS WINDOW_SIZE / 2

typedef struct ship_info_t
{
    int ship;
    int pos_x, pos_y;
    int points;
    movement_t move_type;
} ship_info_t;

typedef struct position_info_t
{
    int pos_x, pos_y;
} position_info_t;

direction_t random_direction()
{
    return random() % 4;
}

void run_aliens(void *responder, void *publisher, WINDOW *space);

void run_players(void *responder, void *publisher, WINDOW *space, WINDOW *score_board);

void run_game(void *responder, void *publisher);

void initialize_connection(void **context, void **responder, void **publisher);

void initialize_ship(ship_info_t *ship_data, position_info_t spawn_point, char ship);

ship_info_t* find_ship_info(ship_info_t ship_data[], int ship);

void initialize_ncurses();

void initialize_window(WINDOW **space, WINDOW **score_board);

int create_new_ship(ship_info_t ship_data[]);

void update_score_board(WINDOW **score_board, ship_info_t ship_data[]);

void new_position(ship_info_t *current_ship, direction_t direction);

void clip_value(int *value, int min, int max);

void update_window_char(WINDOW *space, int x, int y, char c);

void send_msg(void *responder, remote_char_t *m);

void recv_msg(void *responder, remote_char_t *m);

void publish_display_data(void *publisher, ship_info_t *ship_data, char *topic);