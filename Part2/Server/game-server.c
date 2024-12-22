#include "game-server.h"

void *context;
void *lock;

int main()
{
    run_game();
    return 0;
}

/**
 * @brief Creates two different processes, one for the game server and one for the aliens movement.
 */
void run_game()
{
    pthread_t aliens_thread;
    int encryption = random();

    pthread_create(&aliens_thread, NULL, run_aliens, &encryption);

    run_players(encryption);
}

/**
 * @brief Runs the game and handles the communication with players and aliens.
 *   It then processes the messages and updates the display and outer displays accordingly.
 *
 * @param encryption The encryption key used for all the aliens' messages.
 */
void run_players(int encryption)
{
    WINDOW *space, *score_board, *numbers;
    void *context, *responder, *publisher;
    all_ships_t all_ships;
    for (int i = 0; i < N_SHIPS; i++)
    {
        all_ships.ships[i].ship = 0;
    }

    initialize_connection_server(&context, &responder, &publisher);

    initialize_ncurses();

    initialize_window(&space, &score_board, &numbers);

    initialize_aliens(all_ships.aliens, space, encryption);

    remote_char_t m = {};
    int game_end = 0;
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
            if (!game_end) // Block this action if the game has ended
                astronaut_connect(all_ships.ships, &m, space, score_board);
            break;
        case Astronaut_movement:
            if (!game_end) // Block this action if the game has ended
                astronaut_movement(all_ships.ships, &m, space);
            break;
        case Astronaut_zap:
            if (!game_end) // Block this action if the game has ended
                astronaut_zap(&all_ships, &m, space, score_board, publisher);
            break;
        case Astronaut_disconnect:
            astronaut_disconnect(all_ships.ships, &m, space, score_board);
            break;
        case Alien_movement:
            alien_movement(all_ships.aliens, &m, space);
            break;
        case Alien_end:
            end_game_display(space, all_ships);
            game_end = 1;
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

/**
 * @brief Generate the next random move for each alien and send them to the server every second.
 *
 * @param encryption The encryption key used for all the aliens' messages.
 */
void *run_aliens(void *enc_key)
{
    void *context, *requester;
    remote_char_t alien_msg = {};
    int alive[N_ALIENS], game_end = 1;
    int encryption = *(int *)enc_key;

    for (int i = 0; i < N_ALIENS; i++)
    {
        alive[i] = 1;
    }
    context = zmq_ctx_new();
    initialize_connection_client(&context, &requester);

    alien_msg.encryption = encryption;
    alien_msg.action = Alien_movement;
    while (1)
    {
        game_end = 1;
        for (int i = 0; i < N_ALIENS; i++)
        {
            if (alive[i])
            {
                game_end = 0;
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
        if (game_end)
        {
            alien_msg.action = Alien_end; // Send message to the server to end the game if all aliens are dead
            send_TCP(requester, &alien_msg);
            recv_TCP(requester, &alien_msg);
            break;
        }
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    pthread_exit(NULL);
}