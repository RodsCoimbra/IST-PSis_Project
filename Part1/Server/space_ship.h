#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <time.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"

#define N_SHIPS 8
#define RECHARGING_TIME 3
#define STUN_TIME 10

typedef struct ship_info_t
{
    int ship;
    position_info_t position;
    int points;
    movement_t move_type;
    time_t timeouts[2];
} ship_info_t;

typedef enum status_t
{
    RECHARGING,
    STUNNED
} status_t;

void initialize_ship(ship_info_t *, position_info_t, char);

ship_info_t *find_ship_info(ship_info_t[], int);

int create_new_ship(ship_info_t[]);

void new_position(ship_info_t *, direction_t);

void update_score_board(WINDOW **, ship_info_t[]);

#endif
