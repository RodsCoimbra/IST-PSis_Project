#include "astronaut-display-client.h"

void *context;
pthread_mutex_t lock;
pthread_mutex_t lock_space;
pthread_mutex_t lock_aliens;

int main()
{
    void *msg;
    pthread_t joystick_thread;
    long int disconnect = 0;
    context = zmq_ctx_new();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    if (pthread_mutex_init(&lock_space, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    initialize_ncurses();

    pthread_create(&joystick_thread, NULL, joystick, &disconnect);

    display(&disconnect);

    pthread_join(joystick_thread, &msg);

    endwin();

    printf("Disconnected ship %c\n", ((remote_char_t *)msg)->ship);
    printf("Final Pontuation: %d\n", ((remote_char_t *)msg)->points);

    free(msg);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock_space);

    zmq_ctx_destroy(context);

    return 0;
}
