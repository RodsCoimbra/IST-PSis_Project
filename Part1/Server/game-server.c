#include "game-server.h"

int main()
{
    run_game();
    return 0;
}

/*
 * Function: run_game
 *
 *   It creates two different processes, one for the game server and one for the aliens movement.
 *
 */
void run_game()
{
    pid_t pid;
    int encryption = random();
    pid = fork(); // Create a new process
    if (pid < 0)
    {
        perror("fork failed");
        return;
    }
    if (pid == 0) // Child process
    {
        run_aliens(encryption);
    }
    else // Parent process
    {
        run_players(encryption);
    }
}

/*
 * Function: run_players
 *
 *   This function is responsible for running the game server and handling the communication between the players and the aliens.
 *   It then processes the messages and updates the displays accordingly.
 *
 */
void run_players(int encryption)
{
    WINDOW *space, *score_board;
    void *context, *responder, *publisher;
    all_ships_t all_ships;
    for (int i = 0; i < N_SHIPS; i++)
    {
        all_ships.ships[i].ship = 0;
    }

    initialize_connection_server(&context, &responder, &publisher);

    initialize_ncurses();

    initialize_window(&space, &score_board);

    initialize_aliens(all_ships.aliens, space, encryption);

    remote_char_t m = {};
    while (1)
    {
        recv_TCP(responder, &m); // Receive message from client or alien
        // Check if is the correct client or alien sending the message
        if (!check_encryption(all_ships, m))
        {
            send_TCP(responder, &m);
            continue;
        }

        switch (m.action) // Perform the action based on the message received
        {
        case Astronaut_connect:
            astronaut_connect(all_ships.ships, &m, space, score_board);
            break;
        case Astronaut_movement:
            astronaut_movement(all_ships.ships, &m, space);
            break;
        case Astronaut_zap:
            astronaut_zap(&all_ships, &m, space, score_board, publisher);
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
        // Publish the updated display data to the outer-display
        publish_display_data(publisher, &all_ships);

        wrefresh(space);
        wrefresh(score_board);
    }
    endwin();
    zmq_close(responder);
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}

/*
 * Function: run_aliens
 *
 *   This function is responsible for running the aliens movement and sending updates to the server.
 *
 */
void run_aliens(int encryption)
{
    void *context, *requester;
    remote_char_t alien_msg = {};
    int alive[N_ALIENS];
    for (int i = 0; i < N_ALIENS; i++)
    {
        alive[i] = 1;
    }

    initialize_connection_client(&context, &requester);
    alien_msg.encryption = encryption;
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

                if (alien_msg.ship == 0) // if alien_msg.ship == 0, the alien was killed
                    alive[i] = 0;
            }
        }
        sleep(1);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
}