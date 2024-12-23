#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"
#include <unistd.h>
#include "../Utils/communication.h"
#include <pthread.h>

typedef struct alien_info_t alien_info_t;

extern pthread_mutex_t lock_space;
extern pthread_mutex_t lock_aliens;

typedef struct {
    WINDOW *space;
    all_ships_t *all_ships;
    ship_info_t *current_ship;
    void *publisher;
    movement_t direction_zap; 
} thread_display_zap_t;

ship_info_t *find_ship_info(ship_info_t[], int);

int create_new_ship(ship_info_t[]);

void initialize_ship(ship_info_t *, position_info_t, char);

void new_position(ship_info_t *, direction_t);

int IsStunned(ship_info_t *);

int IsRecharging(ship_info_t *, time_t);

void astronaut_connect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void astronaut_movement(ship_info_t *, remote_char_t *, WINDOW *);

void astronaut_zap(all_ships_t *all_ships, remote_char_t *m, WINDOW *space, WINDOW *score_board, void *publisher);

void astronaut_disconnect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void hozirontal_zap(ship_info_t *, all_ships_t* , WINDOW *, int, void *);

void vertical_zap(ship_info_t *, all_ships_t* , WINDOW *, int , void *);

void initialize_ships(ship_info_t *);

#endif
