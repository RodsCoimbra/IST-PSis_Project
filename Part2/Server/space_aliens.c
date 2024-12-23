#include "space_aliens.h"



/**
 * @brief Initializes the aliens and displays them in the outer space
 *
 * @param alien_data The array of aliens to be initialized
 * @param space The window where the aliens will be displayed
 * @param encryption The encryption key used for all the aliens' messages
 */
void initialize_aliens(alien_info_t *alien_data)
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        alien_info_t *current_alien = &(alien_data[i]);
        current_alien->position.x = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->position.y = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
        current_alien->alive = 1;
    }
}

/**
 * @brief Moves the alien in the given direction, deleting the previous position and updating the new one
 *
 * @param TODO
 * @param 
 */
void alien_movement(alien_info_t *current_alien, WINDOW *space, direction_t direction)
{
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_alien->position, ' ');
    pthread_mutex_unlock(&lock_space);
    switch (direction)
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
    // Ensure the alien is within the bounds of the window
    clip_value(&current_alien->position.x, MIN_POS, MAX_POS);
    clip_value(&current_alien->position.y, MIN_POS, MAX_POS);

    // Update the new position of the alien
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_alien->position, '*');
    wrefresh(space);
    pthread_mutex_unlock(&lock_space);
}

/**
 * @brief Generates a random direction for the alien to move
 *
 * @return The random direction generated
 */
direction_t random_direction()
{
    return (direction_t)(random() % 4);
}