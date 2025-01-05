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

/**
 * @brief Receives the key pressed by the user and sets the action of the astronaut.
 *
 * @param m Holds the astronaut message for the server.
 *
 * @return Returns 1 if the key pressed is valid, 0 otherwise.
 */
int execute_action(remote_char_t *m)
{
    int key;
    key = getch();
    switch (key)
    {
    case KEY_LEFT:
        m->direction = LEFT;
        m->action = Astronaut_movement;
        break;
    case KEY_RIGHT:
        m->direction = RIGHT;
        m->action = Astronaut_movement;
        break;
    case KEY_DOWN:
        m->direction = DOWN;
        m->action = Astronaut_movement;
        break;
    case KEY_UP:
        m->direction = UP;
        m->action = Astronaut_movement;
        break;
    case KEY_SPACE:
        m->action = Astronaut_zap;
        break;
    case KEY_q:
    case KEY_Q:
        m->action = Astronaut_disconnect;
        break;
    default:
        return 0;
        break;
    }
    return 1;
}