#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include "remote-char.h"
#include <pthread.h>

extern pthread_mutex_t lock_space;

void clip_value(int *, int, int);

void initialize_ncurses();

int check_encryption(ship_info_t *, remote_char_t );

void initialize_window(WINDOW **, WINDOW **, WINDOW **);

void update_numbers_boxs(WINDOW *, WINDOW *, WINDOW *);

void end_game_display(WINDOW *, all_ships_t);

void update_window_char(WINDOW *, position_info_t, char);

void update_score_board(WINDOW **, ship_info_t[]);

void draw_horizontal(WINDOW *, position_info_t, ship_info_t* , char);

void draw_vertical(WINDOW *, position_info_t, ship_info_t*, char);

int cmp_position(position_info_t , position_info_t );

void refresh_windows(WINDOW *, WINDOW *, WINDOW *);

#endif