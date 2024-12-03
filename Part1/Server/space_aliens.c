#include "space_aliens.h"

void initialize_aliens(alien_info_t alien_data[])
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        alien_data[i].position.x = 3 + random() % (WINDOW_SIZE - 4);
        alien_data[i].position.y = 3 + random() % (WINDOW_SIZE - 4);
        alien_data[i].alive = 1;
    }
}

direction_t random_direction()
{
    return (direction_t)(random() % 4);
}