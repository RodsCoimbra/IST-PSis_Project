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

void alien_movement(alien_info_t alien_data[])
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (alien_data[i].alive)
        {
            direction_t direction = random() % 4;
            switch (direction)
            {
            case UP:
                alien_data[i].position.x--;
                break;
            case DOWN:
                alien_data[i].position.x++;
                break;
            case LEFT:
                alien_data[i].position.y--;
                break;
            case RIGHT:
                alien_data[i].position.y++;
                break;
            }
            clip_value(&alien_data[i].position.x, MAX_POS, MIN_POS);
            clip_value(&alien_data[i].position.y, MAX_POS, MIN_POS);
        }
    }
}