#ifndef SPACE_ALIENS_H
#define SPACE_ALIENS_H

#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"

extern pthread_mutex_t lock_space;

void initialize_aliens(alien_info_t *alien_data);

void alien_movement(alien_info_t *, WINDOW *, direction_t );

direction_t random_direction();

#endif