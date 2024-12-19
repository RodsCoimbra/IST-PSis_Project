#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include "remote-char.h"

void clip_value(int *, int, int);

void initialize_ncurses();

int check_encryption(all_ships_t, remote_char_t);

void initialize_window(WINDOW **, WINDOW **);

void end_game_display(WINDOW *space, all_ships_t all_ships);

void update_window_char(WINDOW *, position_info_t, char);

#endif