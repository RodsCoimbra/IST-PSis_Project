#include "astronaut-display-client.h"

void * context;
pthread_mutex_t lock;

int main()
{
    pthread_t joystick_thread;
    long int disconnect = 0;
    context = zmq_ctx_new();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex has failed\n");
        return 0;
    }

    initialize_ncurses();

    pthread_create(&joystick_thread, NULL, joystick, &disconnect);

    display(&disconnect);
    void *msg;
    pthread_join(joystick_thread, &msg);
    endwin();
    printf("Disconnected ship %c\n", ((remote_char_t *)msg)->ship);
    printf("Final Pontuation: %d\n", ((remote_char_t *)msg)->points);
    free(msg);
    pthread_mutex_destroy(&lock);
    zmq_ctx_destroy(context);

    return 0;
}
