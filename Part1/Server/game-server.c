#include "game-server.h"

int main()
{
    run_game();
    return 0;
}

void run_game()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return;
    }
    if (pid == 0)
    {
        run_aliens();
    }
    else
    {
        run_players();
    }
}

void run_players()
{
    time_t current_time;
    WINDOW *space, *score_board;
    void *context, *responder, *publisher;
    char topic[20] = "Display";
    ship_info_t ship_data[N_SHIPS] = {}, *current_ship = NULL;
    alien_info_t alien_data[N_ALIENS];
    // all_ships_t all_ships;
    // memcpy(all_ships.ships, ship_data, sizeof(all_ships.ships));
    // memcpy(all_ships.aliens, alien_data, sizeof(all_ships.aliens));
    initialize_connection_server(&context, &responder, &publisher);

    initialize_ncurses();

    initialize_window(&space, &score_board);

    initialize_aliens(alien_data, space);

    remote_char_t m = {};
    while (1)
    {
        recv_TCP(responder, &m);

        switch (m.action)
        {
        case Astronaut_connect:
            astronaut_connect(ship_data, &m, space, score_board);
            break;
        case Astronaut_movement:
            astronaut_movement(ship_data, &m, space);
            break;
        case Astronaut_zap:
            astronaut_zap(ship_data, &m, space, alien_data, score_board);
            break;
        case Astronaut_disconnect:
            astronaut_disconnect(ship_data, &m, space, score_board);
            break;
        case Alien_movement:
            alien_movement(alien_data, &m, space);
            break;
        default:
            break;
        }
        send_TCP(responder, &m);

        // publish_display_data(publisher, all_ships, topic);

        wrefresh(space);
        wrefresh(score_board);
    }
    endwin();
    zmq_close(responder);
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}

void run_aliens()
{
    void *context, *requester;
    remote_char_t alien_msg = {};
    int alive[N_ALIENS];
    for (int i = 0; i < N_ALIENS; i++)
    {
        alive[i] = 1;
    }

    initialize_connection_client(&context, &requester);

    alien_msg.action = Alien_movement;
    while (1)
    {
        for (int i = 0; i < N_ALIENS; i++)
        {
            if (alive[i])
            {
                alien_msg.direction = random_direction();
                alien_msg.points = i;
                alien_msg.ship = 1;
                send_TCP(requester, &alien_msg);
                recv_TCP(requester, &alien_msg);
                if (alien_msg.ship == 0)
                    alive[i] = 0;
            }
        }
        sleep(1);
    }
}