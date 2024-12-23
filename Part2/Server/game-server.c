#include "game-server.h"

void *context;
pthread_mutex_t lock_space;
pthread_mutex_t lock_aliens;

int main()
{
    if (pthread_mutex_init(&lock_space, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    if (pthread_mutex_init(&lock_aliens, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    run_game();

    pthread_mutex_destroy(&lock_space);
    pthread_mutex_destroy(&lock_aliens);

    return 0;
}

/**
 * @brief Creates two different processes, one for the game server and one for the aliens movement.
 */
void run_game()
{
    WINDOW *space, *score_board, *numbers;
    pthread_t aliens_thread;
    all_ships_t all_ships;
    all_ships.ships = (ship_info_t *)malloc(N_SHIPS * sizeof(ship_info_t));
    all_ships.aliens = (alien_info_t *)malloc(N_ALIENS * sizeof(alien_info_t));

    initialize_aliens(all_ships.aliens);

    initialize_ships(all_ships.ships);

    initialize_ncurses();

    initialize_window(&space, &score_board, &numbers);

    run_alien_args *args = (run_alien_args*) malloc(sizeof(run_alien_args));
    args->aliens = all_ships.aliens;
    args->space = space;
    pthread_create(&aliens_thread, NULL, run_aliens, args);

    run_players(all_ships, space, score_board);
}

/**
 * @brief Runs the game and handles the communication with players and aliens.
 *   It then processes the messages and updates the display and outer displays accordingly.
 *
 * @param encryption The encryption key used for all the aliens' messages.
 */
void run_players(all_ships_t all_ships, WINDOW *space, WINDOW *score_board)
{
    
    void *responder, *publisher;

    initialize_connection_server(&context, &responder, &publisher);

    remote_char_t m = {};
    int game_end = 0;
    while (1)
    {
        recv_TCP(responder, &m); // Receive message from client or alien
        // Check if is the correct client or alien sending the message
        if (!check_encryption(all_ships.ships, m))
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
        // TODO: Implement the case for the end of the game
        default:
            break;
        }
        send_TCP(responder, &m);

        // Publish the updated display data to the outer-display
        publish_display_data(publisher, &all_ships);

        pthread_mutex_lock(&lock_space);
        wrefresh(space);
        pthread_mutex_unlock(&lock_space);

        wrefresh(score_board);
    }

    free(all_ships.ships);
    free(all_ships.aliens);

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
void *run_aliens(void *args)
{
    run_alien_args *alien_arg = (run_alien_args *)args;
    alien_info_t *alien = alien_arg->aliens;
    int alive, game_end;
    while (1)
    {
        game_end = 1;
        for (int i = 0; i < N_ALIENS; i++)
        {
            pthread_mutex_lock(&lock_aliens);
            alive = alien[i].alive;
            pthread_mutex_unlock(&lock_aliens);

            if (alive)
            {
                game_end = 0;
                alien_movement(&(alien[i]), alien_arg->space, random_direction());
            }
        }
        if (game_end)
            break;

        sleep(1);
    }
    free(args);
    pthread_exit(NULL);
}