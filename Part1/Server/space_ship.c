#include "space_ship.h"

ship_info_t *find_ship_info(ship_info_t ship_data[], int ship)
{
    int num_ship = ship - 'A';

    if (num_ship < 0 || num_ship >= N_SHIPS)
        return NULL;

    if (ship_data[num_ship].ship == 0)
        return NULL;

    return &ship_data[num_ship];
}

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

void initialize_ship(ship_info_t *ship_data, position_info_t spawn_point, char ship)
{
    ship_data->move_type = spawn_point.x == MID_POS ? VERTICAL : HORIZONTAL;
    position_info_t *pos = &ship_data->position;
    pos->x = spawn_point.x;
    pos->y = spawn_point.y;
    ship_data->ship = ship;
    ship_data->points = 0;
    for (int i = 0; i < 2; i++)
    {
        ship_data->timeouts[i] = 0;
    }
}

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
        clip_value(y, MIN_POS, MAX_POS);
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

int IsStunned(ship_info_t *current_ship)
{
    time_t current_time = time(NULL);
    return current_time - current_ship->timeouts[STUNNED] < STUN_TIME;
}

int IsRecharging(ship_info_t *current_ship, time_t current_time)
{
    return current_time - current_ship->timeouts[RECHARGING] < RECHARGING_TIME;
}

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

    update_window_char(space, current_ship->position, m->ship | A_BOLD);

    update_score_board(&score_board, ship_data);
}

void astronaut_movement(ship_info_t *ship_data, remote_char_t *m, WINDOW *space)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);

    if (current_ship == NULL || IsStunned(current_ship))
        return;

    update_window_char(space, current_ship->position, ' ');

    new_position(current_ship, m->direction);

    update_window_char(space, current_ship->position, current_ship->ship | A_BOLD);
}

void astronaut_zap(ship_info_t *ship_data, remote_char_t *m, WINDOW *space, alien_info_t *alien_data, WINDOW *score_board)
{
    ship_info_t *current_ship = find_ship_info(ship_data, m->ship);
    time_t current_time = time(NULL);
    if (current_ship == NULL || IsRecharging(current_ship, current_time))
        return;

    current_ship->timeouts[RECHARGING] = current_time;
    switch (current_ship->move_type)
    {
    case HORIZONTAL:
        hozirontal_zap(current_ship, alien_data, ship_data, space, current_time);
        break;
    case VERTICAL:
        vertical_zap(current_ship, alien_data, ship_data, space, current_time);
        break;
    }
    update_score_board(&score_board, ship_data);
    

}

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

void draw_horizontal(WINDOW *space, position_info_t position, int ship_position, char symbol)
{
    for (position.x = 1; position.x < WINDOW_SIZE - 1; position.x++)
    {
        if (position.x == ship_position)
            continue;
        update_window_char(space, position, symbol);
    }
}

void draw_vertical(WINDOW *space, position_info_t position, int ship_position, char symbol)
{
    for (position.y = 1; position.y < WINDOW_SIZE - 1; position.y++)
    {
        if (position.y == ship_position)
            continue;
        update_window_char(space, position, symbol);
    }
}

void hozirontal_zap(ship_info_t *current_ship, alien_info_t *alien_data, ship_info_t *ship_data, WINDOW *space, int current_time)
{
    position_info_t zap_position = current_ship->position;
    // Draw the zap
    draw_horizontal(space, zap_position, current_ship->position.x, '|');

    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (alien_data[i].alive == 1 && alien_data[i].position.y == zap_position.y)
        {
            alien_data[i].alive = 0;
            current_ship->points++;
        }
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (ship_data[i].ship != 0 && ship_data[i].position.y == zap_position.y && ship_data[i].ship != current_ship->ship)
        {
            ship_data[i].timeouts[STUNNED] = current_time;
        }
    }
    wrefresh(space);

    usleep(500000);

    // Clear the zap
    draw_horizontal(space, zap_position, current_ship->position.x, ' ');

}

void vertical_zap(ship_info_t *current_ship, alien_info_t *alien_data, ship_info_t *ship_data, WINDOW *space, int current_time)
{
    position_info_t zap_position = current_ship->position;
    // Draw the zap
    draw_vertical(space, zap_position, current_ship->position.y, '-');

    // Check for collisions with aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        if (alien_data[i].alive == 1 && alien_data[i].position.x == zap_position.x)
        {
            alien_data[i].alive = 0;
            current_ship->points++;
        }
    }

    // Check for collisions with ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (ship_data[i].ship != 0 && ship_data[i].position.x == zap_position.x && ship_data[i].ship != current_ship->ship)
        {
            ship_data[i].timeouts[STUNNED] = current_time;
        }
    }
    wrefresh(space);
    usleep(500000);

    // Clear the zap
    draw_vertical(space, zap_position, current_ship->position.y, ' ');
}