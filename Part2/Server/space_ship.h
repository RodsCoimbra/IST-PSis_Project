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
extern pthread_mutex_t lock_publish;

typedef struct
{
    WINDOW *space;
    all_ships_t *all_ships;
    ship_info_t *current_ship;
    void *publisher;
} thread_display_zap_t;

ship_info_t *find_ship_info(ship_info_t[], int);

int create_new_ship(ship_info_t[]);

void initialize_ship(ship_info_t *, position_info_t, char);

void new_position(ship_info_t *, direction_t);

int IsStunned(ship_info_t *);

int IsRecharging(ship_info_t *, time_t);

void astronaut_connect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void astronaut_movement(ship_info_t *, remote_char_t *, WINDOW *);

void astronaut_zap(all_ships_t *, remote_char_t *, WINDOW *, WINDOW *, void *);

void astronaut_disconnect(ship_info_t *, remote_char_t *, WINDOW *, WINDOW *);

void zap(ship_info_t *, all_ships_t *, WINDOW *, int, void *, int (*get_position)(position_info_t));

int get_position_x(position_info_t);

int get_position_y(position_info_t);

void initialize_ships(ship_info_t *);

int check_encryption(ship_info_t *, remote_char_t);

#endif
