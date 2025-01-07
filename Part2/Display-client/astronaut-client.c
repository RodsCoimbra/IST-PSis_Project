#include "astronaut-client.h"

/**
 * @brief main client function that handles the astronaut actions.
 *
 * @param arg Pointer to the disconnect flag.
 */
void *joystick(void *arg)
{
    remote_char_t *m = malloc(sizeof(remote_char_t));
    m->points = 0;
    m->ship = ' ';
    int valid_action;
    void *requester;
    long int *disconnect = (long int *)arg;
    long int local_disconnect = 0;
    initialize_connection_client(&context, &requester);

    m->action = Astronaut_connect;

    // first connection to receive encryption key and ship character
    send_TCP(requester, m);
    recv_TCP(requester, m);

    if (m->ship == 0)
    {
        pthread_mutex_lock(&lock);
        *disconnect = 1;
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }

    do
    {
        valid_action = execute_action(m);
        pthread_mutex_lock(&lock);
        local_disconnect = *disconnect;
        pthread_mutex_unlock(&lock);

        if (valid_action && !local_disconnect)
        {
            send_TCP(requester, m);
            recv_TCP(requester, m);
        }

    } while (m->action != Astronaut_disconnect && !local_disconnect);

    pthread_mutex_lock(&lock);
    *disconnect = 1;
    pthread_mutex_unlock(&lock);

    zmq_close(requester);
    return (void *)m;
}