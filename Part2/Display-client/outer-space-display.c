#include "outer-space-display.h"

void display(long int *disconnect, void *subscriber)
{
    WINDOW *space, *score_board, *numbers;
    all_ships_t all_ships;

    all_ships.ships = (ship_info_t *)malloc(msg_ships_size);
    all_ships.aliens = (alien_info_t *)malloc(msg_aliens_size);

    initialize_window(&space, &score_board, &numbers);

    recv_subscription_TCP(subscriber, &all_ships, disconnect);
    long int local_disconnect = 0;
    while (1)
    {
        pthread_mutex_lock(&lock);
        local_disconnect = *disconnect;
        pthread_mutex_unlock(&lock);

        if (local_disconnect)
            break;

        erase_old_data(space, all_ships);

        recv_subscription_TCP(subscriber, &all_ships, disconnect);

        update_numbers_boxs(numbers, space, score_board);

        display_new_data(space, all_ships, score_board, numbers);
    }
    free(all_ships.ships);
    free(all_ships.aliens);
}

/**
 * @brief Erases the old data from the window
 *
 * @param space WINDOW pointer to the game window
 * @param all_ships all_ships_t struct with the data from all ships and aliens
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

/**
 * @brief Displays the new data and zaps in the window
 *
 * @param space WINDOW pointer to the game window
 * @param all_ships all_ships_t struct with the data from all ships and aliens
 * @param score_board WINDOW pointer to the score board window
 */
void display_new_data(WINDOW *space, all_ships_t all_ships, WINDOW *score_board, WINDOW *numbers)
{
    // Display aliens
    int game_end = 1;
    for (int i = 0; i < N_ALIENS; i++)
        if (all_ships.aliens[i].alive)
        {
            game_end = 0;
            update_window_char(space, all_ships.aliens[i].position, '*');
        }

    if (game_end)
        end_game_display(space, all_ships);

    // Display ships and zaps
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships.ships[i].ship != 0)
        {
            update_window_char(space, all_ships.ships[i].position, all_ships.ships[i].ship);
            switch (all_ships.ships[i].zap)
            {
            case DRAW_ZAP:
                draw_zap(space, all_ships.ships[i].zap_position, all_ships.ships[i].move_type, all_ships.ships);
                break;

            case ERASE_ZAP:
                erase_zap(space, all_ships.ships[i].zap_position, all_ships.ships[i].move_type, all_ships.ships);
                break;

            default:
                break;
            }
        }
    }

    update_score_board(&score_board, all_ships.ships);
    wrefresh(space);
    wrefresh(score_board);
    wrefresh(numbers);
}

/**
 * @brief Draws the zap in the window
 *
 * @param space WINDOW pointer to the game window
 * @param position position_info_t struct with the position of the zap
 * @param move_type movement_t enum with the type of movement of the zap
 * @param ships ship_info_t pointer to the ships
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

/**
 * @brief Erases the zap in the window
 *
 * @param space WINDOW pointer to the game window
 * @param position position_info_t struct with the position of the zap
 * @param move_type movement_t enum with the type of movement of the zap
 * @param ships ship_info_t pointer to the ships
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