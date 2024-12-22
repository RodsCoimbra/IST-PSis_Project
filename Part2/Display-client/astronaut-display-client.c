#include "astronaut-display-client.h"

int main()
{
    pthread_t client_thread;

    pthread_create(&client_thread, NULL, client, NULL);

    display();

    return 0;
}