#include "space_ship.h"

/*
 * Function:  find_ship_info
 * 
 * Find the correct ship data based on the received ship character
 *
 *  @param ship_data: array of ship_info_t to search in
 *  @param ship: the ship character we are looking for
 *
 *  returns: pointer to the ship_info_t struct with the correct ship
 */
ship_info_t *find_ship_info(ship_info_t ship_data[], int ship)
{
    int num_ship = ship - 'A';

    if (num_ship < 0 || num_ship >= N_SHIPS)
        return NULL;

    if (ship_data[num_ship].ship == 0)
        return NULL;

    return &ship_data[num_ship];
}

/*
 * Function:  create_new_ship
 * 
 * Find the first available ship slot in the ship_data array
 *
 *  @param ship_data: array of ship_info_t to search in
 *
 *  returns: index of the first available ship slot
 */
int create_new_ship(ship_info_t ship_data[])
{
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (ship_data[i].ship == 0)
            return i;
    }

    // No more ships available
    return -1;
}

/*
 * Function: initialize_ship
 *
 * Initialize the ship data with the correct values
 *
 *  @param ship_data: pointer to the ship_info_t struct to initialize
 *  @param spawn_point: the position_info_t struct with the spawn point of the ship
 *  @param ship: the ship character
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
    {
        ship_data->timeouts[i] = 0;
    }
}

/*
 * Function:  new_position
 * 
 * Update the ship position based on the direction
 *
 *  @param current_ship: pointer to the ship_info_t struct to update
 *  @param direction: the direction_t enum with the direction to move
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

/*
 * Function: update_score_board
 *
 * Update the score board with the current ship data
 *
 *  @param score_board: pointer to the WINDOW struct with the score board window
 *  @param ship_data: array of ship_info_t with the ship data
 */
void update_score_board(WINDOW **score_board, ship_info_t ship_data[])
{
    int score_pos = 3;
    for (int i = 0; i < N_SHIPS; i++)
    {
        ship_info_t current_ship = ship_data[i];
        if (current_ship.ship != 0)
        {
            wmove(*score_board, score_pos, 3);
            wprintw(*score_board, "%c - %d", current_ship.ship, current_ship.points);
            score_pos++;
        }
    }

    // Clear the last entry
    wmove(*score_board, score_pos, 3);
    wprintw(*score_board, "        ");
}

/*
 * Function: IsStunned
 *
 * Check if the ship is stunned
 *
 *  @param current_ship: pointer to the ship_info_t struct to check
 *
 *  returns: 1 if the ship is stunned, 0 otherwise
 */
int IsStunned(ship_info_t *current_ship)
{
    time_t current_time = time(NULL);
    return current_time - current_ship->timeouts[STUNNED] < STUN_TIME;
}

/*
 * Function: IsRecharging
 *
 * Check if the ship is recharging
 *
 *  @param current_ship: pointer to the ship_info_t struct to check
 *
 *  returns: 1 if the ship is recharging, 0 otherwise
 */
int IsRecharging(ship_info_t *current_ship, time_t current_time)
{
    return current_time - current_ship->timeouts[RECHARGING] < RECHARGING_TIME;
}

/*
 * Function: astronaut_connect
 *
 * Connect the astronaut to the ship
 *
 *  @param ship_data: array of ship_info_t with the ship data
 *  @param m: pointer to the remote_char_t struct with the astronaut data
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param score_board: pointer to the WINDOW struct with the score board window
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
    if (ship_idx == -1)
    {
        m->ship = 0;
        return;
    }

    m->ship = 'A' + ship_idx;
    m->points = 0;
    current_ship = &ship_data[ship_idx];

    initialize_ship(current_ship, spawn_points[ship_idx], m->ship);

    m->encryption = current_ship->encryption;

    update_window_char(space, current_ship->position, m->ship | A_BOLD);

    update_score_board(&score_board, ship_data);
}

/*
 * Function: astronaut_movement
 *
 * Move the astronaut to the new position
 *
 *  @param ship_data: array of ship_info_t with the ship data
 *  @param m: pointer to the remote_char_t struct with the astronaut data
 *  @param space: pointer to the WINDOW struct with the space window
 */
void astronaut_movement(ship_info_t *ship_data, remote_char_t *m, WINDOW *space)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);

    if (current_ship == NULL || IsStunned(current_ship))
        return;

    update_window_char(space, current_ship->position, ' ');

    new_position(current_ship, m->direction);

    update_window_char(space, current_ship->position, current_ship->ship | A_BOLD);
}

/*
 * Function: astronaut_zap
 *
 * Fire the zap if the ship is not recharging with the correct direction to all that is in the way
 *
 *  @param all_ships: pointer to the all_ships_t struct with the ship and alien data
 *  @param m: pointer to the remote_char_t struct with the astronaut data
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param score_board: pointer to the WINDOW struct with the score board window
 */
void astronaut_zap(all_ships_t *all_ships, remote_char_t *m, WINDOW *space, WINDOW *score_board, void *publisher)
{
    ship_info_t *current_ship = find_ship_info(all_ships->ships, m->ship);
    time_t current_time = time(NULL);
    if (current_ship == NULL || IsRecharging(current_ship, current_time))
        return;

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
    m->points = current_ship->points;
}

/*
 * Function: astronaut_disconnect
 *
 * Disconnect the astronaut from the game
 *
 *  @param ship_data: array of ship_info_t with the ship data
 *  @param m: pointer to the remote_char_t struct with the astronaut data
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param score_board: pointer to the WINDOW struct with the score board window
 */
void astronaut_disconnect(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, WINDOW *score_board)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);
    if (current_ship == NULL)
        return;

    // delete the ship from the screen
    update_window_char(space, current_ship->position, ' ');

    m->points = current_ship->points;

    current_ship->ship = 0;

    // update the score board
    update_score_board(&score_board, ship_data);
}

/*
 * Function: draw_horizontal
 *
 * Draw the horizontal zap
 *
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param position: position_info_t struct with the position of the zap
 *  @param ship_data: array of ship_info_t with the ship data
 *  @param symbol: the symbol to draw
 */
void draw_horizontal(WINDOW *space, position_info_t position, ship_info_t *ship_data, char symbol)
{
    int draw = 1;
    for (position.x = 1; position.x < WINDOW_SIZE - 1; position.x++, draw = 1)
    {
        if (position.x < MIN_POS || position.x > MAX_POS)
            for (int i = 0; i < N_SHIPS; i++)
                if (ship_data[i].ship != 0 && cmp_position(ship_data[i].position, position))
                {
                    draw = 0;
                    break;
                }
        if (draw)
            update_window_char(space, position, symbol);
    }
}

/*
 * Function: cmp_position
 *
 * Compare two position_info_t structs
 *
 *  @param a: position_info_t struct to compare
 *  @param b: position_info_t struct to compare
 *
 *  returns: 1 if the positions are the same, 0 otherwise
 */
int cmp_position(position_info_t a, position_info_t b)
{
    return a.x == b.x && a.y == b.y;
}

/*
 * Function: draw_vertical
 *
 * Draw the vertical zap
 *
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param position: position_info_t struct with the position of the zap
 *  @param ship_data: array of ship_info_t with the ship data
 *  @param symbol: the symbol to draw
 */
void draw_vertical(WINDOW *space, position_info_t position, ship_info_t *ship_data, char symbol)
{
    int draw = 1;
    for (position.y = 1; position.y < WINDOW_SIZE - 1; position.y++, draw = 1)
    {
        if (position.y < MIN_POS || position.y > MAX_POS)
            for (int i = 0; i < N_SHIPS; i++)
                if (ship_data[i].ship != 0 && cmp_position(ship_data[i].position, position))
                {
                    draw = 0;
                    break;
                }

        if (draw)
            update_window_char(space, position, symbol);
    }
}

/*
 * Function: hozirontal_zap
 *
 * Fire the horizontal zap and check for collisions
 *
 *  @param current_ship: pointer to the ship_info_t struct with the current ship data
 *  @param all_ships: pointer to the all_ships_t struct with the ship and alien data
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param current_time: the current time
 */
void hozirontal_zap(ship_info_t *current_ship, all_ships_t* all_ships, WINDOW *space, int current_time, void *publisher)
{
    position_info_t zap_position = current_ship->position;
    // Draw the zap
    draw_horizontal(space, zap_position, all_ships->ships, '|');
    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (all_ships->aliens[i].alive == 1 && all_ships->aliens[i].position.y == zap_position.y)
        {
            all_ships->aliens[i].alive = 0;
            current_ship->points++;
        }
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships->ships[i].ship != 0 && all_ships->ships[i].position.y == zap_position.y && all_ships->ships[i].ship != current_ship->ship)
            all_ships->ships[i].timeouts[STUNNED] = current_time;
    }
    current_ship->zap = DRAW_ZAP;

    wrefresh(space);
    
    publish_display_data(publisher, all_ships);

    usleep(500000);

    current_ship->zap = ERASE_ZAP;

    // Clear the zap
    draw_horizontal(space, zap_position, all_ships->ships, ' ');

    publish_display_data(publisher, all_ships);

    current_ship->zap = NO_ZAP;
}

/*
 * Function: vertical_zap
 *
 * Fire the vertical zap and check for collisions
 *
 *  @param current_ship: pointer to the ship_info_t struct with the current ship data
 *  @param all_ships: pointer to the all_ships_t struct with the ship and alien data
 *  @param space: pointer to the WINDOW struct with the space window
 *  @param current_time: the current time
 */
void vertical_zap(ship_info_t *current_ship, all_ships_t* all_ships, WINDOW *space, int current_time, void *publisher)
{
    position_info_t zap_position = current_ship->position;
    // Draw the zap
    draw_vertical(space, zap_position, all_ships->ships, '-');

    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (all_ships->aliens[i].alive == 1 && all_ships->aliens[i].position.x == zap_position.x)
        {
            all_ships->aliens[i].alive = 0;
            current_ship->points++;
        }
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships->ships[i].ship != 0 && all_ships->ships[i].position.x == zap_position.x && all_ships->ships[i].ship != current_ship->ship)
            all_ships->ships[i].timeouts[STUNNED] = current_time;
        
    }
    current_ship->zap = DRAW_ZAP;

    wrefresh(space);

    publish_display_data(publisher, all_ships);

    usleep(500000);

    current_ship->zap = ERASE_ZAP;

    // Clear the zap
    draw_vertical(space, zap_position, all_ships->ships, ' ');

    publish_display_data(publisher, all_ships);

    current_ship->zap = NO_ZAP;
}