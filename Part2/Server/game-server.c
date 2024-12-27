#include "game-server.h"

void *context;
pthread_mutex_t lock_space;
pthread_mutex_t lock_aliens;
pthread_mutex_t lock_game_end;

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

    if (pthread_mutex_init(&lock_game_end, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    run_game();

    pthread_mutex_destroy(&lock_space);
    pthread_mutex_destroy(&lock_aliens);
    pthread_mutex_destroy(&lock_game_end);

    return 0;
}

/**
 * @brief Creates two different processes, one for the game server and one for the aliens movement.
 */
void run_game()
{
    WINDOW *space, *score_board, *numbers;
    void *responder, *publisher, *requester;
    pthread_t aliens_thread, keyboard_thread, display_thread;
    int game_end = 0;
    all_ships_t all_ships;
    all_ships.ships = (ship_info_t *)malloc(N_SHIPS * sizeof(ship_info_t));
    all_ships.aliens = (alien_info_t *)malloc(N_ALIENS * sizeof(alien_info_t));

    initialize_aliens(all_ships.aliens);

    initialize_ships(all_ships.ships);

    initialize_connection_server(&context, &responder, &publisher);

    initialize_ncurses();

    initialize_window(&space, &score_board, &numbers);

    // Connections to allow threads to end the game
    initialize_connection_client(&context, &requester);

    // Arguments for the threads
    run_alien_args *args_aliens = (run_alien_args *)malloc(sizeof(run_alien_args));
    args_aliens->game_end = &game_end;
    args_aliens->data = &all_ships;
    args_aliens->space = space;
    args_aliens->publisher = publisher;
    args_aliens->requester = requester;

    display_args *args_display = (display_args *)malloc(sizeof(display_args));
    args_display->space = space;
    args_display->score_board = score_board;
    args_display->numbers = numbers;
    args_display->game_end = &game_end;

    // Threads
    pthread_create(&keyboard_thread, NULL, keyboard_handler, requester);
    pthread_create(&aliens_thread, NULL, run_aliens, args_aliens);
    pthread_create(&display_thread, NULL, thread_refresh, args_display);

    run_players(all_ships, space, score_board, publisher, responder, numbers, &game_end);

    pthread_join(aliens_thread, NULL);
    pthread_join(display_thread, NULL);

    zmq_close(requester);

    free(all_ships.ships);
    free(all_ships.aliens);

    endwin();

    zmq_close(responder);
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}

/**
 * @brief Runs the game and handles the communication with players and aliens.
 *   It then processes the messages and updates the display and outer displays accordingly.
 *
 * @param encryption The encryption key used for all the aliens' messages.
 * TODO
 */
void run_players(all_ships_t all_ships, WINDOW *space, WINDOW *score_board, void *publisher, void *responder, WINDOW *numbers, int *game_end)
{
    remote_char_t m = {};
    while (1)
    {
        recv_TCP(responder, &m); // Receive message from client

        // Check if is the correct client or alien sending the message
        if (!check_encryption(all_ships.ships, m))
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

        case Server_disconnect:
            publish_end_game(publisher);
            pthread_mutex_lock(&lock_game_end);
            *game_end = 1;
            pthread_mutex_unlock(&lock_game_end);
            break;
        default:
            break;
        }
        send_TCP(responder, &m);

        if (*game_end)
            break;

        // Publish the updated display data to the outer-display
        publish_display_data(publisher, &all_ships);
    }
}

/**
 * @brief Generate the next random move for each alien and send them to the server every second.
 *
 * @param encryption The encryption key used for all the aliens' messages.
 */
void *run_aliens(void *args)
{
    run_alien_args *alien_arg = (run_alien_args *)args;
    alien_info_t *aliens = alien_arg->data->aliens;
    void *requester = alien_arg->requester;
    int alive;
    int n_alive;
    int last_num_alive = N_ALIENS;
    int local_game_end;
    time_t revival_timer;

    while (local_game_end == 0)
    {
        n_alive = 0;
        for (int i = 0; i < N_ALIENS; i++)
        {
            pthread_mutex_lock(&lock_aliens);
            alive = aliens[i].alive;
            pthread_mutex_unlock(&lock_aliens);

            if (alive)
            {
                n_alive++;
                alien_movement(&(aliens[i]), alien_arg->space, random_direction());
            }
        }

        alien_recovery(aliens, n_alive, &last_num_alive, &revival_timer);

        publish_display_data(alien_arg->publisher, alien_arg->data);

        sleep(1);

        // End the game if all aliens are dead
        if (n_alive == 0)
        {
            remote_char_t m;
            m.action = Server_disconnect;
            send_TCP(requester, &m);
            recv_TCP(requester, &m);
            break;
        }

        pthread_mutex_lock(&lock_game_end);
        local_game_end = *(alien_arg->game_end);
        pthread_mutex_unlock(&lock_game_end);
    }
    free(args);
    pthread_exit(NULL);
}

void *keyboard_handler(void *requester)
{
    int key;
    remote_char_t m;
    m.action = Server_disconnect;
    while (1)
    {
        key = getch();
        if (key == KEY_q || key == KEY_Q)
        {
            send_TCP(requester, &m);
            recv_TCP(requester, &m);
            break;
        }
    }
    pthread_exit(NULL);
}

void *thread_refresh(void *args)
{
    display_args disp_args = *(display_args *)args;
    WINDOW *space = (WINDOW *)disp_args.space;
    WINDOW *score_board = (WINDOW *)disp_args.score_board;
    WINDOW *numbers = (WINDOW *)disp_args.numbers;
    int *game_end = (int *)disp_args.game_end;
    int local_game_end = 0;

    while (local_game_end == 0)
    {
        pthread_mutex_lock(&lock_game_end);
        local_game_end = *game_end;
        pthread_mutex_unlock(&lock_game_end);

        update_numbers_boxs(numbers, space, score_board);
        pthread_mutex_lock(&lock_space);
        refresh_windows(space, score_board, numbers);
        pthread_mutex_unlock(&lock_space);

        usleep(300);
    }
    free(args);
    pthread_exit(NULL);
}