#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include "remote-char.h"

void clip_value(int *, int, int);

void initialize_ncurses();

void initialize_window(WINDOW **, WINDOW **);

void update_window_char(WINDOW *, position_info_t, char);

#endif