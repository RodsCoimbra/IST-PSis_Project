#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"

typedef struct alien_info_t alien_info_t;

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
