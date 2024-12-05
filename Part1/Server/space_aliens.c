#include "space_aliens.h"

/*
 * Function: initialize_aliens
 *
 * Initializes the aliens in the outer space
 * @param alien_data: array of alien_info_t to store the aliens data
 * @param space: window where the aliens will be displayed
 */
void initialize_aliens(alien_info_t *alien_data, WINDOW *space, int encryption)
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        alien_info_t *current_alien = &(alien_data[i]);
        current_alien->position.x = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->position.y = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->alive = 1;
        current_alien->encryption = encryption;
        update_window_char(space, current_alien->position, '*');
        wrefresh(space);
    }
}

/*
 * Function: alien_movement
 *
 * Processes the movement of the aliens by deleting the current position and updating the new one
 * @param alien_data: array of alien_info_t to store the alien data
 * @param m: remote_char_t with the direction of the alien movement
 * @param space: window where the aliens will be displayed
*/
void alien_movement(alien_info_t *alien_data, remote_char_t *m, WINDOW *space)
{
    int alien_idx = m->points;
    alien_info_t *current_alien = &(alien_data[alien_idx]);
    if (!current_alien->alive) // If the alien is dead, do nothing
    {
        m->ship = 0;
        return;
    }
    // Delete the current position of the alien
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
    // Update the new position of the alien
    update_window_char(space, current_alien->position, '*');
}

/*
 * Function: random_direction
 *
 * Generates a random direction for the alien to move
*/
direction_t random_direction()
{
    return (direction_t)(random() % 4);
}