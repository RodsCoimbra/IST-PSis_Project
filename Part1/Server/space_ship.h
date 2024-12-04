#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"
#include <unistd.h>

typedef struct alien_info_t alien_info_t;

ship_info_t *find_ship_info(ship_info_t[], int);

int create_new_ship(ship_info_t[]);

void initialize_ship(ship_info_t *, position_info_t, char);

void new_position(ship_info_t *, direction_t);

void update_score_board(WINDOW **, ship_info_t[]);

int IsStunned(ship_info_t *);

int IsRecharging(ship_info_t *, time_t);

void astronaut_connect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void astronaut_movement(ship_info_t *, remote_char_t *, WINDOW *);

void astronaut_zap(ship_info_t *, remote_char_t *, WINDOW *, alien_info_t *, WINDOW *);

void astronaut_disconnect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void hozirontal_zap(ship_info_t *, alien_info_t *, ship_info_t *, WINDOW *, int );

void vertical_zap(ship_info_t *, alien_info_t *, ship_info_t *, WINDOW *space, int current_time);

void draw_horizontal(WINDOW *space, position_info_t position, ship_info_t* ship_data, char symbol);

int cmp_position(position_info_t a, position_info_t b);

void draw_vertical(WINDOW *space, position_info_t position, ship_info_t* ship_data, char symbol);

#endif
