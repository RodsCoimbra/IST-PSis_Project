#include "astronaut-display-client.h"

void *context;
pthread_mutex_t lock;
pthread_mutex_t lock_space;
pthread_mutex_t lock_aliens;

int main()
{
    void *msg;
    pthread_t joystick_thread, disconnect_server_thread;
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

    pthread_create(&disconnect_server_thread, NULL, disconnect_server, &disconnect);

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


void * disconnect_server(void *arg)
{
    long int *disconnect = (long int *)arg;
    void *sock_pull = zmq_socket (context, ZMQ_PULL);
    int rc = zmq_bind (sock_pull, TCP_PATH_PULL);
    assert (rc == 0);
    int n = 0;
    zmq_recv(sock_pull, &n, sizeof(n), 0);
    if (n == 1)
    {
        pthread_mutex_lock(&lock);
        *disconnect = 1;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}