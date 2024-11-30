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

typedef struct ship_info_t
{
    int ship;
    int pos_x, pos_y;
    int points;
    direction_t side;
} ship_info_t;

typedef struct alien_info_t
{
    int pos_x, pos_y;
} alien_info_t;

direction_t random_direction()
{
    return random() % 4;
}

void new_position(int *x, int *y, direction_t direction, direction_t side);

int find_ship_info(ship_info_t ship_data[], int n_ships, int ship);

void initialize_ncurses();

void initialize_window(WINDOW **space, WINDOW **score_board);

char find_new_ship(ship_info_t ship_data[], int n_ships);

void update_score_board(WINDOW **score_board, ship_info_t ship_data[], int n_ships);

void random_position(int *x, int *y, direction_t direction);