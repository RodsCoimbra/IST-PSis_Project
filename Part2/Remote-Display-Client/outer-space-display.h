#ifndef OUTER_SPACE_DISPLAY_H
#define OUTER_SPACE_DISPLAY_H

#include "string.h"
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "../Utils/Utils.h"
#include "../Server/space_ship.h"
#include "../Server/space_aliens.h"

void receive_ships(void *subscriber, all_ships_t *all_ships);

void erase_old_data(WINDOW *space, all_ships_t all_ships);

void display_new_data(WINDOW *space, all_ships_t all_ships, WINDOW *score_board);

void draw_zap(WINDOW *space, position_info_t position, movement_t move_type, ship_info_t *ships);

void erase_zap(WINDOW *space, position_info_t position, movement_t move_type, ship_info_t *ships);
#endif