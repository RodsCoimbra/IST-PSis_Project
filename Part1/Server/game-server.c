#include "game-server.h"

int main()
{
    void *context, *responder, *publisher;
    initialize_connection(&context, &responder, &publisher);

    run_game(responder, publisher);

    zmq_close(responder);
    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}

void run_game(void *responder, void *publisher)
{
    WINDOW *space, *score_board;
    pid_t pid;

    initialize_ncurses();

    initialize_window(&space, &score_board);

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }
    if (pid == 0)
    {
        run_aliens(responder, publisher, space);
    }
    else
    {
        run_players(responder, publisher, space, score_board);
    }
}

void run_aliens(void *responder, void *publisher, WINDOW *space)
{
    position_info_t alien_data[N_ALIENS];

    initialize_aliens(alien_data);

    while (1)
    {
        alien_movement(alien_data);
        sleep(1);
        // update_window_char(space, MID_POS + random() % 2 - 1, MID_POS + random() % 2 - 1, '*');
    }
}

void alien_movement(position_info_t alien_data[])
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        int direction = random() % 4;
        switch (direction)
        {
        case UP:
            alien_data[i].pos_x--;
            break;
        case DOWN:
            alien_data[i].pos_x++;
            break;
        case LEFT:
            alien_data[i].pos_y--;
            break;
        case RIGHT:
            alien_data[i].pos_y++;
            break;
        }
        clip_value(&alien_data[i].pos_x, MAX_POS, MIN_POS);
        clip_value(&alien_data[i].pos_y, MAX_POS, MIN_POS);
    }
}

void run_players(void *responder, void *publisher, WINDOW *space, WINDOW *score_board)
{
    ship_info_t ship_data[N_SHIPS] = {}, *current_ship = NULL;

    // TODO: MUDAR QUANDO SE mudar O SIZE ECRã
    position_info_t spawn_points[] = {{1, MID_POS},
                                      {WINDOW_SIZE - 2, MID_POS},
                                      {MID_POS, 1},
                                      {MID_POS, WINDOW_SIZE - 2},
                                      {2, MID_POS},
                                      {WINDOW_SIZE - 3, MID_POS},
                                      {MID_POS, 2},
                                      {MID_POS, WINDOW_SIZE - 3}};

    remote_char_t m = {};
    while (1)
    {
        recv_msg(responder, &m);
        switch (m.action)
        {
        case Astronaut_connect:
        {
            int ship_idx = create_new_ship(ship_data);
            if (ship_idx == -1)
            {
                m.ship = 0;
                send_msg(responder, &m);
                break;
            }
            m.ship = 'A' + ship_idx;
            m.points = 0;
            current_ship = &ship_data[ship_idx];

            initialize_ship(current_ship, spawn_points[ship_idx], m.ship);

            update_window_char(space, current_ship->pos_x, current_ship->pos_y, m.ship | A_BOLD);

            update_score_board(&score_board, ship_data);

            send_msg(responder, &m);
        }
        break;

        case Astronaut_movement:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            if (current_ship == NULL)
                break;

            update_window_char(space, current_ship->pos_x, current_ship->pos_y, ' ');

            new_position(current_ship, m.direction);

            update_window_char(space, current_ship->pos_x, current_ship->pos_y, current_ship->ship | A_BOLD);

            send_msg(responder, &m);
        }
        break;

        case Astronaut_zap:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            if (current_ship == NULL)
                break;

            switch (current_ship->move_type)
            {
            case HORIZONTAL:
            {
                for (int i = 1; i < WINDOW_SIZE - 1; i++)
                {
                    update_window_char(space, i, current_ship->pos_y, '|');
                }
            }
            break;
            case VERTICAL:
            {
                for (int i = 1; i < WINDOW_SIZE - 1; i++)
                {
                    update_window_char(space, current_ship->pos_x, i, '-');
                }
            }
            break;
            default:
                break;
            }
            // TODO: IF ALIEN in the way, kill it and update score
            send_msg(responder, &m);
        }

        break;

        case Astronaut_disconnect:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            if (current_ship == NULL)
                break;
            // delete the ship from the screen
            update_window_char(space, current_ship->pos_x, current_ship->pos_y, ' ');

            current_ship->ship = 0;

            // update the score board
            update_score_board(&score_board, ship_data);

            send_msg(responder, &m);
        }
        break;

        default:
            break;
        }

        publish_display_data(publisher, ship_data, "display");

        wrefresh(space);
        wrefresh(score_board);
    }
    endwin();
}

void initialize_connection(void **context, void **responder, void **publisher)
{
    *context = zmq_ctx_new();
    // Responder
    *responder = zmq_socket(*context, ZMQ_REP);
    int rc = zmq_bind(*responder, TCP_PATH_REP);
    assert(rc == 0);
    // Publisher
    *publisher = zmq_socket(*context, ZMQ_PUB);
    rc = zmq_bind(*publisher, TCP_PATH_PUB);
    assert(rc == 0);
}

void initialize_aliens(position_info_t alien_data[])
{
    for (int i = 0; i < N_ALIENS; i++)
    {
        alien_data[i].pos_x = 3 + random() % (WINDOW_SIZE - 4);
        alien_data[i].pos_y = 3 + random() % (WINDOW_SIZE - 4);
    }
}

void new_position(ship_info_t *current_ship, direction_t direction)
{
    int *y, *x;
    switch (current_ship->move_type)
    {
    case HORIZONTAL:
        y = &current_ship->pos_y;
        if (direction == LEFT)
            (*y)--;
        else if (direction == RIGHT)
            (*y)++;
        clip_value(y, MIN_POS, MAX_POS);
        break;

    case VERTICAL:
        x = &current_ship->pos_x;
        if (direction == UP)
            (*x)--;
        else if (direction == DOWN)
            (*x)++;
        clip_value(x, MIN_POS, MAX_POS);
        break;
    }
}

ship_info_t *find_ship_info(ship_info_t ship_data[], int ship)
{
    int num_ship = ship - 'A';

    if (num_ship < 0 || num_ship >= N_SHIPS)
        return NULL;

    if (ship_data[num_ship].ship == 0)
        return NULL;

    return &ship_data[num_ship];
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

void initialize_ship(ship_info_t *ship_data, position_info_t spawn_point, char ship)
{
    ship_data->move_type = spawn_point.pos_x == MID_POS ? VERTICAL : HORIZONTAL;
    ship_data->pos_x = spawn_point.pos_x;
    ship_data->pos_y = spawn_point.pos_y;
    // TODO: Mudar para position_info_t o ship_data_pos
    ship_data->ship = ship;
    ship_data->points = 0;
}

void clip_value(int *value, int min, int max)
{
    if (*value < min)
        *value = min;

    else if (*value > max)
        *value = max;
}

void update_window_char(WINDOW *space, int x, int y, char c)
{
    wmove(space, x, y);
    waddch(space, c);
}

void send_msg(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}

void recv_msg(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

void publish_display_data(void *publisher, ship_info_t *ship_data, char *topic)
{
    if (zmq_send(publisher, topic, strlen(topic), ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    if (zmq_send(publisher, ship_data, sizeof(ship_info_t) * N_SHIPS, 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}