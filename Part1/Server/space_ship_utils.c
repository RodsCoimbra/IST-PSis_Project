#include "space_ship_utils.h"

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