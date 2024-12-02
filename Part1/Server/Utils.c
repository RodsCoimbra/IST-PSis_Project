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
    // TODO: COLOCAR NUMEROS AO LADO DA JANELA
    *space = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    *score_board = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, WINDOW_SIZE + 2);
    box(*space, 0, 0);
    box(*score_board, 0, 0);

    wmove(*score_board, 2, 3);
    wprintw(*score_board, "SCORE");

    wrefresh(*space);
    wrefresh(*score_board);
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