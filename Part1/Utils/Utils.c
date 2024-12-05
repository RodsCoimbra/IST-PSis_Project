#include "Utils.h"

void initialize_ncurses()
{
    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
}

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

int check_encryption(all_ships_t all_ships, remote_char_t m)
{
    if (m.action == Astronaut_connect)
        return 1;
    
    if(m.action == Alien_movement)
    {
        if (m.encryption == all_ships.aliens[0].encryption) //all aliens have the same encryption
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
    return 0;
}

void clip_value(int *value, int min, int max)
{
    if (*value < min)
        *value = min;

    else if (*value > max)
        *value = max;
}

void update_window_char(WINDOW *space, position_info_t update_position, char c)
{
    wmove(space, update_position.x, update_position.y);
    waddch(space, c);
}