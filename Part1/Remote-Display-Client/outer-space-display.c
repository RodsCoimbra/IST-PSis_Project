#include "outer-space-display.h"

/*
 * Function: main
 *
 * Subscribes to the server and displays the game
 */
int main()
{
    void *context, *subscriber;
    WINDOW *space, *score_board;
    all_ships_t all_ships;

    char topic[20] = "Display";
    initialize_connection_sub(&context, &subscriber, topic);

    initialize_ncurses();

    initialize_window(&space, &score_board);

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

/*
 * Function: erase_old_data
 *
 * Erases the old data from the window
 * @param space: WINDOW pointer to the game window
 * @param all_ships: all_ships_t struct with the ships and aliens
 */
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

/*
 * Function: display_new_data
 *
 * Displays the new data in the window and zaps
 * @param space: WINDOW pointer to the game window
 * @param all_ships: all_ships_t struct with the ships and aliens
 * @param score_board: WINDOW pointer to the score board window
 */
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

/*
 * Function: draw_zap
 *
 * Draws the zap in the window
 * @param space: WINDOW pointer to the game window
 * @param position: position_info_t struct with the position of the zap
 * @param move_type: movement_t enum with the type of movement of the zap
 * @param ships: ship_info_t pointer to the ships
 */
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

/*
 * Function: erase_zap
 *
 * Erases the zap in the window
 * @param space: WINDOW pointer to the game window
 * @param position: position_info_t struct with the position of the zap
 * @param move_type: movement_t enum with the type of movement of the zap
 * @param ships: ship_info_t pointer to the ships
 */
void erase_zap(WINDOW *space, position_info_t position, movement_t move_type, ship_info_t *ships)
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