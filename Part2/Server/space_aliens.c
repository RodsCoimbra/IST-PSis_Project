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
 * @param current_alien The alien to be moved
 * @param space The window where the alien is displayed
 * @param direction The direction in which the alien will move
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

/**
 * @brief Revives the alien in a random position
 *
 * @param alien The alien to be revived
 */
void revive_alien(alien_info_t *alien)
{
    int x = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;
    int y = random() % (MAX_POS - MIN_POS + 1) + MIN_POS;

    pthread_mutex_lock(&lock_aliens);
    alien->alive = 1;
    alien->position.x = x;
    alien->position.y = y;
    pthread_mutex_unlock(&lock_aliens);
}

/**
 * @brief Revives 10% of the dead aliens after a certain amount of time
 *
 * @param aliens The array of aliens
 * @param n_alive The number of alive aliens
 * @param last_num_alive The number of alive aliens in the previous iteration
 * @param revival_timer The time when the last alien was killed
 */
void alien_recovery(alien_info_t *aliens, int n_alive, int *last_num_alive, time_t *revival_timer)
{
    // If the number of alive aliens has changed, reset the timer
    if (*last_num_alive != n_alive)
    {
        *last_num_alive = n_alive;
        *revival_timer = time(NULL);
        return;
    }

    // If 10 seconds have not passed since the last alien was killed, return
    time_t current_time = time(NULL);
    if (current_time - *revival_timer < REVIVE_TIME)
        return;

    // revive 10% of the dead aliens
    int revive_number = (int)(n_alive / 10);
    int alive;
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (!revive_number)
            break;

        pthread_mutex_lock(&lock_aliens);
        alive = aliens[i].alive;
        pthread_mutex_unlock(&lock_aliens);

        if (!alive)
        {
            revive_alien(&(aliens[i]));
            revive_number--;
        }
    }
}