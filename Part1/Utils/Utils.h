#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include "remote-char.h"

#define WINDOW_SIZE 22
#define MAX_POS WINDOW_SIZE - 4
#define MIN_POS 3
#define MID_POS WINDOW_SIZE / 2

void clip_value(int *, int, int);

void initialize_ncurses();

void initialize_window(WINDOW **, WINDOW **);

void update_window_char(WINDOW *, position_info_t, char);

#endif