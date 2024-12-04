#include "space_aliens.h"

void initialize_aliens(alien_info_t *alien_data, WINDOW *space)
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        alien_info_t *current_alien = &(alien_data[i]);
        current_alien->position.x = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->position.y = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->alive = 1;
        update_window_char(space, current_alien->position, '*');
        wrefresh(space);
    }
}

void alien_movement(alien_info_t *alien_data, remote_char_t *m, WINDOW *space)
{
    int alien_idx = m->points;
    alien_info_t *current_alien = &(alien_data[alien_idx]);
    if (!current_alien->alive)
    {
        m->ship = 0;
        return;
    }

    update_window_char(space, current_alien->position, ' ');
    switch (m->direction)
    {
    case UP:
        current_alien->position.x--;
        break;
    case DOWN:
        current_alien->position.x++;
        break;
    case LEFT:
        current_alien->position.y--;
        break;
    case RIGHT:
        current_alien->position.y++;
        break;
    default:
        break;
    }
    clip_value(&current_alien->position.x, MIN_POS, MAX_POS);
    clip_value(&current_alien->position.y, MIN_POS, MAX_POS);
    update_window_char(space, current_alien->position, '*');
}

direction_t random_direction()
{
    return (direction_t)(random() % 4);
}