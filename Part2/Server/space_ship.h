#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"
#include <unistd.h>
#include "../Utils/communication.h"

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

void astronaut_zap(all_ships_t *all_ships, remote_char_t *m, WINDOW *space, WINDOW *score_board, void *publisher);

void astronaut_disconnect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void hozirontal_zap(ship_info_t *, all_ships_t* , WINDOW *, int, void *);

void vertical_zap(ship_info_t *, all_ships_t* , WINDOW *, int , void *);

void draw_horizontal(WINDOW *, position_info_t, ship_info_t* , char);

int cmp_position(position_info_t a, position_info_t b);

void draw_vertical(WINDOW *, position_info_t, ship_info_t*, char);

#endif
