#include "astronaut-display-client.h"

void *context;
pthread_mutex_t lock;
pthread_mutex_t lock_space;

int main()
{
    long int disconnect = 0;
    void *subscriber;
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

    initialize_connection_sub(&context, &subscriber);

    display(&disconnect, subscriber);

    endwin();
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock_space);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}

