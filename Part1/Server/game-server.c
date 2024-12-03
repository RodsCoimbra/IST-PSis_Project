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
    run_players(responder, publisher);
    // pid_t pid;
    // pid = fork();
    // if (pid < 0)
    // {
    //     perror("fork failed");
    //     return;
    // }
    // if (pid == 0)
    // {
    //     // run_aliens(*context, *requester);
    //     int a = 0;
    //     a = 1;
    // }
    // else
    // {
    //     run_players(responder, publisher);
    // }
}

void run_players(void *responder, void *publisher)
{
    WINDOW *space, *score_board;

    initialize_ncurses();

    initialize_window(&space, &score_board);

    ship_info_t ship_data[N_SHIPS] = {}, *current_ship = NULL;
    alien_info_t alien_data[N_ALIENS];

    all_ships_t all_ships = {ship_data, alien_data};

    initialize_aliens(alien_data);

    time_t current_time;

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
            astronaut_zap(ship_data, &m, space, alien_data);
            break;
        case Astronaut_disconnect:
            astronaut_disconnect(ship_data, &m, space, score_board);
            break;
        case Alien_movement:
            
            //alien_movement(alien_data);
            break;
        default:
            break;
        }
        send_TCP(responder, &m);
        
        publish_display_data(publisher, ship_data, "Astronaut");

        wrefresh(space);
        wrefresh(score_board);
    }
    endwin();
}

// void run_aliens(void *context, void *requester)
// {
//     initialize_connection_client(context, requester);

//     remote_char_t alien_msg[N_ALIENS] = {};
//     alien_msg.action = Alien_movement;
//     alien_msg.ship = '*';

//     while (1)
//     {
//         for (int i = 0; i < N_ALIENS; i++)
//         {
//             alien_msg.direction = random_direction();

//             send_TCP(requester, &alien_msg[i]);

//             recv_TCP(requester, &alien_msg[i]);
//         }

//         sleep(1);
//     }
// }