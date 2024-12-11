#include "Utils.h"

/**
 * @brief Initialize the ncurses
 */
void initialize_ncurses()
{
    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
    curs_set(0);          /* Do not display the cursor */
}

/**
 * @brief Initialize the game window and the score board window
 *
 * @param space Pointer to the game window
 * @param score_board Pointer to the score board window
 */
void initialize_window(WINDOW **space, WINDOW **score_board)
{
    WINDOW *numbers = newwin(WINDOW_SIZE + 3, WINDOW_SIZE + 3, 0, 0);
    *space = newwin(WINDOW_SIZE, WINDOW_SIZE, 1, 1);
    *score_board = newwin(WINDOW_SIZE, WINDOW_SIZE, 1, WINDOW_SIZE + 3);

    box(*space, 0, 0);
    box(*score_board, 0, 0);

    wmove(*score_board, 2, 3);
    wprintw(*score_board, "SCORE");

    for (int i = 1; i <= WINDOW_SIZE - 2; i++)
    {
        mvwprintw(numbers, i + 1, 0, "%d", i % 10); // Left side numbers
        mvwprintw(numbers, 0, i + 1, "%d", i % 10); // Top side numbers
    }

    wrefresh(numbers);
    wrefresh(*space);
    wrefresh(*score_board);
}

/**
 * @brief Checks if the encryption of the message is correct to avoid cheating
 *
 * @param all_ships all ships data
 * @param m message to check
 * @return 1 if the encryption is correct, 0 otherwise
 */
int check_encryption(all_ships_t all_ships, remote_char_t m)
{
    // Check if the message is the first connection, no encryption needed
    if (m.action == Astronaut_connect)
        return 1;

    // Check if the message is from an alien or a ship
    if (m.action == Alien_movement || m.action == Alien_end)
    {
        if (m.encryption == all_ships.aliens[0].encryption) // all aliens have the same encryption
            return 1;
    }
    else
    {
        for (int i = 0; i < N_SHIPS; i++)
        {
            if (all_ships.ships[i].encryption == m.encryption)
                return 1;
        }
    }

    // If the encryption is not valid, ignore message
    return 0;
}

/**
 * @brief Clip the value between the min and max values
 *
 * @param value value to clip
 * @param min minimum value
 * @param max maximum value
 */
void clip_value(int *value, int min, int max)
{
    if (*value < min)
        *value = min;

    else if (*value > max)
        *value = max;
}

/**
 * @brief Update the character in the window
 *
 * @param space Pointer to the game window
 * @param update_position Position to update
 * @param c Character to update
 */

void update_window_char(WINDOW *space, position_info_t update_position, char c)
{
    wmove(space, update_position.x, update_position.y);
    waddch(space, c);
}

/**
 * @brief Displays who wins the game and its score
 *
 * @param space WINDOW pointer to the game window
 * @param all_ships all_ships_t struct with the data from all ships and aliens
 * @param score_board WINDOW pointer to the score board window
 */
void end_game_display(WINDOW *space, all_ships_t all_ships)
{
    static int max = -1, max_index = -1;
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships.ships[i].ship != 0 && all_ships.ships[i].points > max)
        {
            max = all_ships.ships[i].points;
            max_index = i;
        }
    }
    char msg_display[100];
    sprintf(msg_display, "PLAYER %c WON!!", all_ships.ships[max_index].ship);
    wmove(space, MIN_POS, MIN_POS);
    waddstr(space, msg_display);
    sprintf(msg_display, "SCORE: %d", all_ships.ships[max_index].points);
    wmove(space, MIN_POS + 1, MIN_POS);
    waddstr(space, msg_display);
}