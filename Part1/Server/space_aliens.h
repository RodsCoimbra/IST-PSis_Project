#ifndef SPACE_ALIENS_H
#define SPACE_ALIENS_H

#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"

#define N_ALIENS ((WINDOW_SIZE - 2) * (WINDOW_SIZE - 2) / 3)

typedef struct alien_info_t
{
    position_info_t position;
    int alive;
} alien_info_t;

void initialize_aliens(alien_info_t[]);

void alien_movement(alien_info_t[]);

#endif