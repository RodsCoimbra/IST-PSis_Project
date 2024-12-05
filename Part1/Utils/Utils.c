#include "Utils.h"

/*
 * Function: initialize_ncurses
 *
 * Initializes the ncurses mode
 */
void initialize_ncurses()
{
    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
}

/*
 * Function: initialize_window
 *
 * Initializes the game window and the score board window
 * @param space: WINDOW pointer to the game window
 * @param score_board: WINDOW pointer to the score board window
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

/*
 * Function: check_encryption
 *
 * Checks if the encryption of the message is valid to avoid cheating
 * @param all_ships: all_ships_t struct with the ships and aliens
 * @param m: remote_char_t struct with the message
 * @return 1 if the encryption is valid, 0 otherwise
 */
int check_encryption(all_ships_t all_ships, remote_char_t m)
{
    // Check if the message is the first connection, no encryption needed
    if (m.action == Astronaut_connect)
        return 1;

    // Check if the encryption is the same as the ship or alien
    if (m.action == Alien_movement)
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

/*
 * Function: clip_value
 *
 * Clips the value between the min and max values
 * @param value: int pointer to the value
 * @param min: int with the minimum value
 * @param max: int with the maximum value
 */
void clip_value(int *value, int min, int max)
{
    if (*value < min)
        *value = min;

    else if (*value > max)
        *value = max;
}

/*
 * Function: update_window_char
 *
 * Updates the character in the window
 * @param space: WINDOW pointer to the game window
 * @param update_position: position_info_t struct with the position to update
 * @param c: char with the character to update
 */
void update_window_char(WINDOW *space, position_info_t update_position, char c)
{
    wmove(space, update_position.x, update_position.y);
    waddch(space, c);
}