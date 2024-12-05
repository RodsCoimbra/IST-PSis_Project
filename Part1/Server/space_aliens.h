#ifndef SPACE_ALIENS_H
#define SPACE_ALIENS_H

#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/Utils.h"

void initialize_aliens(alien_info_t *alien_data, WINDOW *space, int encryption);

void alien_movement(alien_info_t *, remote_char_t *, WINDOW *);

direction_t random_direction();

#endif