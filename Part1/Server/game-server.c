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
    ship_info_t *current_ship = NULL;
    all_ships_t all_ships;
    for (int i = 0; i < N_SHIPS; i++)
    {
        all_ships.ships[i].ship = 0;
    }
    initialize_connection_server(&context, &responder, &publisher);

    initialize_ncurses();

    initialize_window(&space, &score_board);

    initialize_aliens(all_ships.aliens, space);

    remote_char_t m = {};
    while (1)
    {
        recv_TCP(responder, &m);

        switch (m.action)
        {
        case Astronaut_connect:
            astronaut_connect(all_ships.ships, &m, space, score_board);
            break;
        case Astronaut_movement:
            astronaut_movement(all_ships.ships, &m, space);
            break;
        case Astronaut_zap:
            astronaut_zap(all_ships.ships, &m, space, all_ships.aliens, score_board);
            break;
        case Astronaut_disconnect:
            astronaut_disconnect(all_ships.ships, &m, space, score_board);
            break;
        case Alien_movement:
            alien_movement(all_ships.aliens, &m, space);
            break;
        default:
            break;
        }
        send_TCP(responder, &m);

        publish_display_data(publisher, &all_ships);

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