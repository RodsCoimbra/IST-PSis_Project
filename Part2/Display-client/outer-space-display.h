#ifndef OUTER_SPACE_DISPLAY_H
#define OUTER_SPACE_DISPLAY_H

#include "string.h"
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "../Utils/Utils.h"
#include "../Server/space_ship.h"
#include "../Server/space_aliens.h"
#include <pthread.h>
extern void *context;
extern pthread_mutex_t lock;

void display(long int *, void *);

void receive_ships(void *, all_ships_t *);

void erase_old_data(WINDOW *, all_ships_t);

void display_new_data(WINDOW *, all_ships_t, WINDOW *, WINDOW *);

void draw_zap(WINDOW *, position_info_t, movement_t, ship_info_t *);

void erase_zap(WINDOW *, position_info_t, movement_t, ship_info_t *);
#endif