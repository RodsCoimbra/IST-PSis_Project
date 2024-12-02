#ifndef UTILS_H
#define UTILS_H

#include "../remote-char.h"
#include <ncurses.h>

#define WINDOW_SIZE 20
#define MAX_POS WINDOW_SIZE - 4
#define MIN_POS 3
#define MID_POS WINDOW_SIZE / 2

void clip_value(int *value, int min, int max);

void initialize_ncurses();

void initialize_window(WINDOW **space, WINDOW **score_board);

void update_window_char(WINDOW *space, position_info_t update_position, char c);

#endif