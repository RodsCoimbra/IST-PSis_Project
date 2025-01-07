#include "client.h"

pthread_mutex_t lock_space; // Just for compatibility with the communication libraryç
pthread_mutex_t lock;       // Mutex for the disconnect flag
void *context;

/**
 * @brief Client of the game, receives commands for movement and zapping of a single astronaut and sends it to the server
 *
 * @return Returns 0 if the program ends successfully.
 */
int main()
{
    void *msg;
    pthread_t joystick_thread;
    long int disconnect = 0;
    long int local_disconnect = 0;
    void *subscriber;
    context = zmq_ctx_new();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    initialize_ncurses();

    // Initialize the connection to the subscriber
    initialize_connection_sub(&context, &subscriber);

    pthread_create(&joystick_thread, NULL, joystick, &disconnect);

    while (local_disconnect == 0)
    {
        recv_subscription_TCP(subscriber, NULL, &disconnect);

        pthread_mutex_lock(&lock);
        local_disconnect = disconnect;
        pthread_mutex_unlock(&lock);
    }

    pthread_join(joystick_thread, &msg);
    endwin();

    printf("Disconnected ship %c\n", ((remote_char_t *)msg)->ship);
    printf("Final Pontuation: %d\n", ((remote_char_t *)msg)->points);

    free(msg);

    pthread_mutex_destroy(&lock);

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
