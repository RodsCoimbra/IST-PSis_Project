#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <time.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"
#include "space_aliens.h"

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

ship_info_t *find_ship_info(ship_info_t[], int);

int create_new_ship(ship_info_t[]);

void initialize_ship(ship_info_t *, position_info_t, char);

void new_position(ship_info_t *, direction_t);

void update_score_board(WINDOW **, ship_info_t[]);

int IsStunned(ship_info_t *current_ship);

int IsRecharging(ship_info_t *current_ship, time_t current_time);

void astronaut_connect(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, WINDOW *score_board);

void astronaut_movement(ship_info_t *ship_data, remote_char_t *m, WINDOW *space);

void astronaut_zap(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, alien_info_t *alien_data);

void astronaut_disconnect(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, WINDOW *score_board);

void hozirontal_zap(ship_info_t *current_ship, alien_info_t *alien_data, ship_info_t *ship_data, WINDOW *space, int current_time);

void vertical_zap(ship_info_t *current_ship, alien_info_t *alien_data, ship_info_t *ship_data, WINDOW *space, int current_time);
#endif
