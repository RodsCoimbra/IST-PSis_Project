#include "game-server.h"

int main()
{
    void *context, *responder, *publisher;
    initialize_connection_server(&context, &responder, &publisher);

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
    int fd_read, fd_write;
    alien_info_t alien_data[N_ALIENS];

    // fd_write = create_write_FIFO(FIFO_LOCATION_CHILD_PARENT);

    // fd_read = create_read_FIFO(FIFO_LOCATION_PARENT_CHILD);

    initialize_aliens(alien_data);

    while (1)
    {
        alien_movement(alien_data);

        // write_FIFO(fd_write, alien_data);

        // recv_FIFO(fd_read, alien_data);

        sleep(1);
    }
}

void run_players(void *responder, void *publisher, WINDOW *space, WINDOW *score_board)
{
    ship_info_t ship_data[N_SHIPS] = {}, *current_ship = NULL;
    alien_info_t alien_data[N_ALIENS];
    time_t current_time;
    int fd_read, fd_write;

    // fd_read = create_read_FIFO(FIFO_LOCATION_CHILD_PARENT);
    // fd_write = create_write_FIFO(FIFO_LOCATION_PARENT_CHILD);
    remote_char_t m = {};
    while (1)
    {
        recv_TCP(responder, &m);

        // recv_FIFO(fd_read, alien_data);

        switch (m.action)
        {
        case Astronaut_connect:
        {
            astronaut_connect(ship_data, &m, space, score_board);
        }
        break;
        case Astronaut_movement:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            current_time = time(NULL);
            if (current_ship == NULL || current_time - current_ship->timeouts[STUNNED] < STUN_TIME)
                break;

            update_window_char(space, current_ship->position, ' ');

            new_position(current_ship, m.direction);

            update_window_char(space, current_ship->position, current_ship->ship | A_BOLD);
        }
        break;

        case Astronaut_zap:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            current_time = time(NULL);
            if (current_ship == NULL || current_time - current_ship->timeouts[RECHARGING] < RECHARGING_TIME)
                break;

            current_ship->timeouts[RECHARGING] = current_time;
            position_info_t zap_position = current_ship->position;
            switch (current_ship->move_type)
            {
            case HORIZONTAL:
            {
                for (int i = 1; i < WINDOW_SIZE - 1; i++)
                {
                    zap_position.x = i;
                    if (current_ship->position.x == zap_position.x)
                        continue;
                    update_window_char(space, zap_position, '|');
                }
                for (int i = 0; i < N_ALIENS; i++)
                {
                    if (alien_data[i].position.y == zap_position.y)
                    {
                        alien_data[i].alive = 0;
                        current_ship->points++;
                    }
                }
                for (int i = 0; i < N_SHIPS; i++)
                {
                    if (ship_data[i].ship != 0 && ship_data[i].position.y == zap_position.y && ship_data[i].ship != current_ship->ship)
                    {
                        ship_data[i].timeouts[STUNNED] = current_time;
                    }
                }
            }
            break;
            case VERTICAL:
            {
                for (int i = 1; i < WINDOW_SIZE - 1; i++)
                {
                    zap_position.y = i;
                    if (current_ship->position.y == zap_position.y)
                        continue;
                    update_window_char(space, zap_position, '-');
                }
                for (int i = 0; i < N_ALIENS; i++)
                {
                    if (alien_data[i].position.x == zap_position.x)
                    {
                        alien_data[i].alive = 0;
                        current_ship->points++;
                    }
                }
                for (int i = 0; i < N_SHIPS; i++)
                {
                    if (ship_data[i].ship != 0 && ship_data[i].position.x == zap_position.x && ship_data[i].ship != current_ship->ship)
                    {
                        ship_data[i].timeouts[STUNNED] = current_time;
                    }
                }
            }
            break;
            default:
                break;
            }

            // write_FIFO(fd_write, alien_data);
        }
        break;

        case Astronaut_disconnect:
        {
            current_ship = find_ship_info(ship_data, m.ship);
            if (current_ship == NULL)
                break;

            // delete the ship from the screen
            update_window_char(space, current_ship->position, ' ');

            current_ship->ship = 0;

            // update the score board
            update_score_board(&score_board, ship_data);
        }
        break;
        default:
            break;
        }
        send_TCP(responder, &m);

        // publish_display_data(publisher, ship_data, "display");

        wrefresh(space);
        wrefresh(score_board);
    }
    endwin();
}

void astronaut_connect(ship_info_t *ship_data, remote_char_t* m, WINDOW *space, WINDOW *score_board)
{
    ship_info_t *current_ship = NULL;
    // TODO: MUDAR QUANDO SE mudar O SIZE ECRã
    position_info_t spawn_points[] = {{1, MID_POS},
                                      {WINDOW_SIZE - 2, MID_POS},
                                      {MID_POS, 1},
                                      {MID_POS, WINDOW_SIZE - 2},
                                      {2, MID_POS},
                                      {WINDOW_SIZE - 3, MID_POS},
                                      {MID_POS, 2},
                                      {MID_POS, WINDOW_SIZE - 3}};

    int ship_idx = create_new_ship(ship_data);
    if (ship_idx == -1)
    {
        m->ship = 0;
        return;
    }

    m->ship = 'A' + ship_idx;
    m->points = 0;
    current_ship = &ship_data[ship_idx];

    initialize_ship(current_ship, spawn_points[ship_idx], m->ship);

    update_window_char(space, current_ship->position, m->ship | A_BOLD);

    update_score_board(&score_board, ship_data);
}
