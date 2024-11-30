#include "game-server.h"

int main()
{
    WINDOW *space, *score_board;
    ship_info_t ship_data[N_SHIPS];
    alien_info_t alien_data[N_ALIENS];

    int side_count[8] = {UP, DOWN, LEFT, RIGHT, UP, DOWN, LEFT, RIGHT};
    int n_ships = 0;

    void *context = zmq_ctx_new();

    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, TCP_PATH_REP);

    void *publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, TCP_PATH_PUB);

    initialize_ncurses();

    initialize_window(&space, &score_board);

    int ship;
    int pos_x;
    int pos_y;

    direction_t direction;

    remote_char_t m;
    while (1)
    {
        if (zmq_recv(responder, &m, sizeof(remote_char_t), 0) == -1)
            exit(0);

        if (m.action == Astronaut_connect) /* CONNECTION */
        {
            m.ship = find_new_ship(ship_data, n_ships);
            m.action = Astronaut_movement;
            m.points = 0;

            ship_data[n_ships].side = side_count[n_ships];
            random_position(&pos_x, &pos_y, ship_data[n_ships].side);
            ship_data[n_ships].ship = m.ship;
            ship_data[n_ships].pos_x = pos_x;
            ship_data[n_ships].pos_y = pos_y;
            ship_data[n_ships].points = 0;

            n_ships++;

            ship = m.ship;
            pos_x = WINDOW_SIZE / 2;
            pos_y = WINDOW_SIZE / 2;

            zmq_send(responder, &m, sizeof(m), 0);
        }
        if (m.action == Astronaut_movement) /* MOVEMENT */
        {
            int ship_pos = find_ship_info(ship_data, n_ships, m.ship);
            if (ship_pos != -1)
            {
                pos_x = ship_data[ship_pos].pos_x;
                pos_y = ship_data[ship_pos].pos_y;
                ship = ship_data[ship_pos].ship;
                /*deletes old place */
                wmove(space, pos_x, pos_y);
                waddch(space, ' ');

                /* claculates new direction */
                direction = m.direction;

                /* calculates new mark position */
                new_position(&pos_x, &pos_y, direction, ship_data[ship_pos].side);
                ship_data[ship_pos].pos_x = pos_x;
                ship_data[ship_pos].pos_y = pos_y;

                /* draw mark on new position */
                wmove(space, pos_x, pos_y);
                waddch(space, ship | A_BOLD);

                wmove(score_board, ship_pos + 3, 3);
                wprintw(score_board, "%c - %d", ship, m.points);

                zmq_send(responder, &m, sizeof(m), 0);
            }
        }
        if (m.action == Astronaut_zap) /* ZAP */
        {
        }
        if (m.action == Astronaut_disconnect) /* DISCONNECTION */
        {
            int ship_pos = find_ship_info(ship_data, n_ships, m.ship);
            if (ship_pos != -1)
            {
                pos_x = ship_data[ship_pos].pos_x;
                pos_y = ship_data[ship_pos].pos_y;
                ship = ship_data[ship_pos].ship;
                /*deletes old place */
                wmove(space, pos_x, pos_y);
                waddch(space, ' ');

                for (int i = ship_pos; i < n_ships - 1; i++)
                {
                    ship_data[i] = ship_data[i + 1];
                }
                n_ships--;
            }
            update_score_board(&score_board, ship_data, n_ships);
            zmq_send(responder, &m, sizeof(m), 0);
        }

        zmq_send(publisher, "display\0", 8, ZMQ_SNDMORE);
        zmq_send(publisher, &ship_data, sizeof(ship_info_t) * 100, 0);

        wrefresh(space);
        wrefresh(score_board);
    }

    endwin(); /* End curses mode*/
    zmq_close(responder);
    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}

void new_position(int *x, int *y, direction_t direction, direction_t side)
{
    switch (side)
    {
    case UP:
        switch (direction)
        {
        case UP:
            (*x)--;
            if (*x == 0)
                *x = 1;
            break;
        case DOWN:
            (*x)++;
            if (*x == 3)
                *x = 2;
            break;
        case LEFT:
            (*y)--;
            if (*y == 2)
                *y = 3;
            break;
        case RIGHT:
            (*y)++;
            if (*y == WINDOW_SIZE - 3)
                *y = WINDOW_SIZE - 4;
            break;
        default:
            break;
        }
        break;
    case DOWN:
        switch (direction)
        {
        case UP:
            (*x)--;
            if (*x == WINDOW_SIZE - 4)
                *x = WINDOW_SIZE - 3;
            break;
        case DOWN:
            (*x)++;
            if (*x == WINDOW_SIZE - 1)
                *x = WINDOW_SIZE - 2;
            break;
        case LEFT:
            (*y)--;
            if (*y == 2)
                *y = 3;
            break;
        case RIGHT:
            (*y)++;
            if (*y == WINDOW_SIZE - 3)
                *y = WINDOW_SIZE - 4;
            break;
        default:
            break;
        }
        break;
    case LEFT:
        switch (direction)
        {
        case UP:
            (*x)--;
            if (*x == 2)
                *x = 3;
            break;
        case DOWN:
            (*x)++;
            if (*x == WINDOW_SIZE - 3)
                *x = WINDOW_SIZE - 4;
            break;
        case LEFT:
            (*y)--;
            if (*y == 0)
                *y = 1;
            break;
        case RIGHT:
            (*y)++;
            if (*y == 3)
                *y = 2;
            break;
        default:
            break;
        }
        break;
    case RIGHT:
        switch (direction)
        {
        case UP:
            (*x)--;
            if (*x == 2)
                *x = 3;
            break;
        case DOWN:
            (*x)++;
            if (*x == WINDOW_SIZE - 3)
                *x = WINDOW_SIZE - 4;
            break;
        case LEFT:
            (*y)--;
            if (*y == WINDOW_SIZE - 4)
                *y = WINDOW_SIZE - 3;
            break;
        case RIGHT:
            (*y)++;
            if (*y == WINDOW_SIZE - 1)
                *y = WINDOW_SIZE - 2;
            break;
        default:
            break;
        }
    }
}

int find_ship_info(ship_info_t ship_data[], int n_ships, int ship)
{
    for (int i = 0; i < n_ships; i++)
    {
        if (ship == ship_data[i].ship)
        {
            return i;
        }
    }
    return -1;
}

void initialize_ncurses()
{
    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
}

void initialize_window(WINDOW **space, WINDOW **score_board)
{
    *space = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    *score_board = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, WINDOW_SIZE + 2);
    box(*space, 0, 0);
    box(*score_board, 0, 0);

    wmove(*score_board, 2, 3);
    wprintw(*score_board, "SCORE");

    wrefresh(*space);
    wrefresh(*score_board);
}

char find_new_ship(ship_info_t ship_data[], int n_ships)
{
    char ship = 'A';
    for (int i = 0; i < n_ships; i++)
    {
        if (ship == ship_data[i].ship)
        {
            ship++;
        }
    }
    return ship;
}

void update_score_board(WINDOW **score_board, ship_info_t ship_data[], int n_ships)
{
    for (int i = 0; i < N_SHIPS; i++)
    {
        if (i < n_ships)
        {
            wmove(*score_board, i + 3, 3);
            wprintw(*score_board, "%c - %d", ship_data[i].ship, ship_data[i].points);
        }
        else
        {
            wmove(*score_board, i + 3, 3);
            wprintw(*score_board, "        ");
        }
    }
}

void random_position(int *x, int *y, direction_t side)
{
    switch (side)
    {
    case UP:
        *x = 1;
        *y = random() % (WINDOW_SIZE - 2) + 1;
        break;
    case DOWN:
        *x = WINDOW_SIZE - 2;
        *y = random() % (WINDOW_SIZE - 2) + 1;
        break;
    case LEFT:
        *x = random() % (WINDOW_SIZE - 2) + 1;
        *y = 1;
        break;
    case RIGHT:
        *x = random() % (WINDOW_SIZE - 2) + 1;
        *y = WINDOW_SIZE - 2;
        break;
    default:
        break;
    }
}