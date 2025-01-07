#include "client.h"

pthread_mutex_t lock_space; // Just for compatibility with the communication library
void *context;

/**
 * @brief Client of the game, receives commands for movement and zapping of a single astronaut and sends it to the server
 *
 * @return Returns 0 if the program ends successfully.
 */
int main()
{
    remote_char_t m;
    int valid_action;
    long int disconnect = 0;

    void *requester;
    context = zmq_ctx_new();

    initialize_connection_client(&context, &requester);

    pthread_t thread;
    pthread_create(&thread, NULL, thread_disconnect, &disconnect);
    m.action = Astronaut_connect;
    // first connection to receive encryption key and ship character
    send_TCP(requester, &m);

    recv_TCP(requester, &m);
    if (m.ship == 0)
    {
        printf("Server is full\n");
        return 0;
    }
    initialize_ncurses();
    do
    {
        valid_action = execute_action(&m);
        if (valid_action && !(disconnect))
        {
            send_TCP(requester, &m);
            recv_TCP(requester, &m);
            mvprintw(0, 0, "Ship %c with pontuation: %d", m.ship, m.points);
        }
        refresh(); /* Print it on to the real screen */
    } while (m.action != Astronaut_disconnect && !disconnect);

    endwin(); /* End curses mode*/
    pthread_join(thread, NULL);
    zmq_close(requester);
    zmq_ctx_destroy(context);

    printf("Disconnected ship %c\n", m.ship);
    printf("Final Pontuation: %d\n", m.points);
    return 0;
}

/**
 * @brief Disconnect thread, receives the disconnection message from the server
 *
 * @param arg Pointer to the disconnect flag
 */
void *thread_disconnect(void *arg)
{
    char topic[] = "Disconn";
    void *subscriber;
    long int *disconnect = (long int *)arg;

    subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, TCP_PATH_SUB);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));

    recv_subscription_TCP(subscriber, NULL, disconnect);

    zmq_close(subscriber);

    pthread_exit(NULL);
}