#include "outer-space-display.h"

int main()
{
    void *context, *subscriber;
    WINDOW *space, *score_board;

    char topic[20] = "Display";
    initialize_connection_sub(&context, &subscriber, topic);

    initialize_ncurses();

    initialize_window(&space, &score_board);
    all_ships_t all_ships;

    recv_subscription_TCP(subscriber, &all_ships);

    while (1)
    {
        erase_old_data(space, all_ships);

        recv_subscription_TCP(subscriber, &all_ships);

        display_new_data(space, all_ships, score_board);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);
    return 0;
}

void erase_old_data(WINDOW *space, all_ships_t all_ships)
{
    // Erase ships
    for (int i = 0; i < N_SHIPS; i++)
        if (all_ships.ships[i].ship != 0)
            update_window_char(space, all_ships.ships[i].position, ' ');

    // Erase aliens
    for (int i = 0; i < N_ALIENS; i++)
        update_window_char(space, all_ships.aliens[i].position, ' ');
}

void display_new_data(WINDOW *space, all_ships_t all_ships, WINDOW *score_board)
{
    // Display aliens
    for (int i = 0; i < N_ALIENS; i++)
        if (all_ships.aliens[i].alive)
            update_window_char(space, all_ships.aliens[i].position, '*');

    // Display ships and zaps
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships.ships[i].ship != 0)
        {
            update_window_char(space, all_ships.ships[i].position, all_ships.ships[i].ship);
            switch (all_ships.ships[i].zap)
            {
            case DRAW_ZAP:
                draw_zap(space, all_ships.ships[i].position, all_ships.ships[i].move_type, all_ships.ships);
                break;

            case ERASE_ZAP:
                erase_zap(space, all_ships.ships[i].position, all_ships.ships[i].move_type, all_ships.ships);
                break;

            default:
                break;
            }
        }
    }

    update_score_board(&score_board, all_ships.ships);
    wrefresh(space);
    wrefresh(score_board);
}

void draw_zap(WINDOW *space, position_info_t position, movement_t move_type, ship_info_t *ships)
{
    switch (move_type)
    {
    case HORIZONTAL:
        draw_horizontal(space, position, ships, '|');
        break;

    case VERTICAL:
        draw_vertical(space, position, ships, '-');
        break;
    }
}

void erase_zap(WINDOW *space, position_info_t position, movement_t move_type , ship_info_t *ships)
{
    switch (move_type)
    {
    case HORIZONTAL:
        draw_horizontal(space, position, ships, ' ');
        break;

    case VERTICAL:
        draw_vertical(space, position, ships, ' ');
        break;
    }
}