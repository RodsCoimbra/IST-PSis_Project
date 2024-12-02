#ifndef SPACE_ALIENS_UTILS_H
#define SPACE_ALIENS_UTILS_H

#include "../remote-char.h"
#include "Utils.h"
#include <stdlib.h>

#define N_ALIENS ((WINDOW_SIZE - 2) * (WINDOW_SIZE - 2) / 3)

typedef struct alien_info_t
{
    position_info_t position;
    int alive;
} alien_info_t;

void initialize_aliens(alien_info_t alien_data[]);

void alien_movement(alien_info_t alien_data[]);

#endif