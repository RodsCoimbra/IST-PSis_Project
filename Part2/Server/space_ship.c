#include "space_ship.h"

/**
 * @brief Find the correct ship data based on the ship character
 *
 * @param ship_data Array of ship data
 * @param ship Ship character we are searching for
 *
 * @return Pointer to the ship_info_t struct of the ship character, NULL otherwise
 */
ship_info_t *find_ship_info(ship_info_t ship_data[], int ship)
{
    int num_ship = ship - 'A';

    if (num_ship < 0 || num_ship >= N_SHIPS)
        return NULL;

    // The ship we are looking for isn't connected
    if (ship_data[num_ship].ship == 0)
        return NULL;

    return &ship_data[num_ship];
}

/**
 * @brief Create a new ship, if there is an available slot
 *
 * @param ship_data Array of ship data
 *
 * @return Index of the new ship, -1 if there are no available slots
 */
int create_new_ship(ship_info_t ship_data[])
{
    for (int i = 0; i < N_SHIPS; i++)
    {
        // In the ship_data, ship = 0 means that the ship is available
        if (ship_data[i].ship == 0)
            return i;
    }

    // No more ships available
    return -1;
}

/**
 * @brief Initialize the ship data
 *
 * @param ship_data Pointer to the ship_info_t struct of the ship
 * @param spawn_point Position where the ship will spawn
 * @param ship Ship character
 */
void initialize_ship(ship_info_t *ship_data, position_info_t spawn_point, char ship)
{
    ship_data->move_type = spawn_point.x == MID_POS ? VERTICAL : HORIZONTAL;
    position_info_t *pos = &ship_data->position;
    pos->x = spawn_point.x;
    pos->y = spawn_point.y;
    ship_data->ship = ship;
    ship_data->points = 0;
    ship_data->encryption = random();
    ship_data->zap = NO_ZAP;

    for (int i = 0; i < 2; i++)
        ship_data->timeouts[i] = 0;
}

/**
 * @brief Update the position of the ship based on the direction
 *
 * @param current_ship Pointer to the ship_info_t struct of the ship to update
 * @param direction Direction of the movement
 */
void new_position(ship_info_t *current_ship, direction_t direction)
{
    int *y, *x;
    position_info_t *pos = &current_ship->position;

    switch (current_ship->move_type)
    {
    case HORIZONTAL:
        y = &pos->y;
        if (direction == LEFT)
            (*y)--;
        else if (direction == RIGHT)
            (*y)++;
        clip_value(y, MIN_POS, MAX_POS); // check if the new position is within the window
        break;

    case VERTICAL:
        x = &pos->x;
        if (direction == UP)
            (*x)--;
        else if (direction == DOWN)
            (*x)++;
        clip_value(x, MIN_POS, MAX_POS);
        break;
    }
}



/**
 * @brief Check if the ship is stunned
 *
 * @param current_ship Pointer to the ship_info_t struct of the ship
 *
 * @return 1 if the ship is stunned, 0 otherwise
 */
int IsStunned(ship_info_t *current_ship)
{
    time_t current_time = time(NULL);
    return current_time - current_ship->timeouts[STUNNED] < STUN_TIME;
}

/**
 * @brief Check if the ship is recharging
 *
 * @param current_ship Pointer to the ship_info_t struct of the ship
 * @param current_time Current time
 *
 * @return 1 if the ship is recharging, 0 otherwise
 */
int IsRecharging(ship_info_t *current_ship, time_t current_time)
{
    return current_time - current_ship->timeouts[RECHARGING] < RECHARGING_TIME;
}

/**
 * @brief Connect the astronaut to its ship
 *
 * @param ship_data Array of ship data
 * @param m Pointer to the message of the astronaut
 * @param space Pointer to the game window
 * @param score_board Pointer to the score board window
 */
void astronaut_connect(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, WINDOW *score_board)
{
    ship_info_t *current_ship = NULL;
    position_info_t spawn_points[] = {{1, MID_POS},
                                      {WINDOW_SIZE - 2, MID_POS},
                                      {MID_POS, 1},
                                      {MID_POS, WINDOW_SIZE - 2},
                                      {2, MID_POS},
                                      {WINDOW_SIZE - 3, MID_POS},
                                      {MID_POS, 2},
                                      {MID_POS, WINDOW_SIZE - 3}};

    int ship_idx = create_new_ship(ship_data);
    if (ship_idx == -1) // if there are no available slots
    {
        m->ship = 0;
        return;
    }

    m->ship = 'A' + ship_idx;
    m->points = 0;
    current_ship = &ship_data[ship_idx];

    initialize_ship(current_ship, spawn_points[ship_idx], m->ship);

    m->encryption = current_ship->encryption; // send the encryption key to the client
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_ship->position, m->ship | A_BOLD);
    pthread_mutex_unlock(&lock_space);

    update_score_board(&score_board, ship_data);
}

/**
 * @brief Move the astronaut's ship
 *
 * @param ship_data Array of ship data
 * @param m Pointer to the message of the astronaut
 * @param space Pointer to the game window
 */
void astronaut_movement(ship_info_t *ship_data, remote_char_t *m, WINDOW *space)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);

    // If the ship is not connected or is stunned, do nothing
    if (current_ship == NULL || IsStunned(current_ship))
        return;

    // Delete the ship from the screen
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_ship->position, ' ');
    pthread_mutex_unlock(&lock_space);

    // Update the position
    new_position(current_ship, m->direction);

    // Draw the ship in the new position
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_ship->position, current_ship->ship | A_BOLD);
    pthread_mutex_unlock(&lock_space);
}

/**
 * @brief Fire the zap if the ship is not recharging
 *
 * @param all_ships Pointer to the all_ships_t struct with all the ship and alien data
 * @param m Pointer to the message of the astronaut
 * @param space Pointer to the game window
 * @param score_board Pointer to the score board window
 * @param publisher Pointer to the publisher
 */
void astronaut_zap(all_ships_t *all_ships, remote_char_t *m, WINDOW *space, WINDOW *score_board, void *publisher)
{
    time_t current_time = time(NULL);
    ship_info_t *current_ship = find_ship_info(all_ships->ships, m->ship);

    // If the ship is not connected or is recharging, do nothing
    if (current_ship == NULL || IsRecharging(current_ship, current_time))
        return;

    // Update the recharging timeout
    current_ship->timeouts[RECHARGING] = current_time;

    switch (current_ship->move_type)
    {
    case HORIZONTAL:
        hozirontal_zap(current_ship, all_ships, space, current_time, publisher);
        break;
    case VERTICAL:
        vertical_zap(current_ship, all_ships, space, current_time, publisher);
        break;
    }

    update_score_board(&score_board, all_ships->ships);

    // Update the points of the client
    m->points = current_ship->points;
}

/**
 * @brief Disconnect the astronaut from the game
 *
 * @param ship_data Array of ship data
 * @param m Pointer to the message of the astronaut
 * @param space Pointer to the game window
 * @param score_board Pointer to the score board window
 */
void astronaut_disconnect(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, WINDOW *score_board)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);
    if (current_ship == NULL)
        return;

    // delete the ship from the screen
    pthread_mutex_lock(&lock_space);
    update_window_char(space, current_ship->position, ' ');
    pthread_mutex_unlock(&lock_space);

    // send the final pontuation to the client
    m->points = current_ship->points;

    // Flag that the ship is disconnected
    current_ship->ship = 0;

    // update the score board
    update_score_board(&score_board, ship_data);
}

void *display_zap(void *arg)
{
    thread_display_zap_t *display_zap_arg = (thread_display_zap_t *)arg;
    display_zap_arg->current_ship->zap = DRAW_ZAP;
    void (*draw)(WINDOW *, position_info_t, ship_info_t *, char);
    char symbol;
    switch (display_zap_arg->current_ship->move_type)
    {
    case HORIZONTAL:
        draw = draw_horizontal;
        symbol = '|';
        break;

    case VERTICAL:
        draw = draw_vertical;
        symbol = '-';
        break;

    default:
        pthread_exit(NULL);
    }
    display_zap_arg->current_ship->zap_position = display_zap_arg->current_ship->position;
    
    // Draw the zap
    draw(display_zap_arg->space, display_zap_arg->current_ship->zap_position, display_zap_arg->all_ships->ships, symbol);
    
    pthread_mutex_lock(&lock_space);
    wrefresh(display_zap_arg->space);
    pthread_mutex_unlock(&lock_space);
    
    // send an update to the outer-display to display the zap
    publish_display_data(display_zap_arg->publisher, display_zap_arg->all_ships);
    
    // zap is displayed for 0.5 seconds
    usleep(500000);
    
    // Clear the zap
    draw(display_zap_arg->space, display_zap_arg->current_ship->zap_position, display_zap_arg->all_ships->ships, ' ');
    
    // send an update to the outer-display to erase the zap
    display_zap_arg->current_ship->zap = ERASE_ZAP;
    publish_display_data(display_zap_arg->publisher, display_zap_arg->all_ships);

    pthread_mutex_lock(&lock_space);
    wrefresh(display_zap_arg->space);
    pthread_mutex_unlock(&lock_space);

    display_zap_arg->current_ship->zap = NO_ZAP;
    free(display_zap_arg);

    pthread_exit(NULL);
}

/**
 * @brief Fire the horizontal zap and check for collisions
 *
 * @param current_ship Pointer to the ship_info_t struct of the ship
 * @param all_ships Pointer to the all_ships_t struct with all the ship and alien data
 * @param space Pointer to the game window
 * @param current_time Current time
 * @param publisher Pointer to the publisher
 */
void hozirontal_zap(ship_info_t *current_ship, all_ships_t *all_ships, WINDOW *space, int current_time, void *publisher)
{
    position_info_t zap_position = current_ship->position;

    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        pthread_mutex_lock(&lock_aliens);
        if (all_ships->aliens[i].alive == 1 && all_ships->aliens[i].position.y == zap_position.y)
        {
            all_ships->aliens[i].alive = 0;
            current_ship->points++;
        }
        pthread_mutex_unlock(&lock_aliens);
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships->ships[i].ship != 0 && all_ships->ships[i].position.y == zap_position.y && all_ships->ships[i].ship != current_ship->ship)
            all_ships->ships[i].timeouts[STUNNED] = current_time;
    }

    pthread_t thread;

    thread_display_zap_t *display_zap_arg = (thread_display_zap_t *)malloc(sizeof(thread_display_zap_t));
    display_zap_arg->space = space;
    display_zap_arg->all_ships = all_ships;
    display_zap_arg->current_ship = current_ship;
    display_zap_arg->publisher = publisher;

    pthread_create(&thread, NULL, display_zap, (void *)display_zap_arg);
}

/**
 * @brief Fire the vertical zap and check for collisions
 *
 * @param current_ship Pointer to the ship_info_t struct of the ship
 * @param all_ships Pointer to the all_ships_t struct with all the ship and alien data
 * @param space Pointer to the game window
 * @param current_time Current time
 * @param publisher Pointer to the publisher
 */
void vertical_zap(ship_info_t *current_ship, all_ships_t *all_ships, WINDOW *space, int current_time, void *publisher)
{
    position_info_t zap_position = current_ship->position;

    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        pthread_mutex_lock(&lock_aliens);
        if (all_ships->aliens[i].alive == 1 && all_ships->aliens[i].position.x == zap_position.x)
        {
            all_ships->aliens[i].alive = 0;
            current_ship->points++;
        }
        pthread_mutex_unlock(&lock_aliens);
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships->ships[i].ship != 0 && all_ships->ships[i].position.x == zap_position.x && all_ships->ships[i].ship != current_ship->ship)
            all_ships->ships[i].timeouts[STUNNED] = current_time;
    }
    current_ship->zap = DRAW_ZAP;

    pthread_t thread;

    thread_display_zap_t *display_zap_arg = (thread_display_zap_t *)malloc(sizeof(thread_display_zap_t));
    display_zap_arg->space = space;
    display_zap_arg->all_ships = all_ships;
    display_zap_arg->current_ship = current_ship;
    display_zap_arg->publisher = publisher;

    pthread_create(&thread, NULL, display_zap, (void *)display_zap_arg);
}

void initialize_ships(ship_info_t *ship_data)
{
    for (int i = 0; i < N_SHIPS; i++)
        ship_data[i].ship = 0;
}