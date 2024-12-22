#include "communication.h"

/**
 * @brief Initialize the connection for the server
 *
 * @param context Pointer to the zmq context
 * @param responder Pointer to the zmq responder socket
 * @param publisher Pointer to the zmq publisher socket
 */
void initialize_connection_server(void **context, void **responder, void **publisher)
{
    // Create new zmq context
    *context = zmq_ctx_new();

    // Initialize responder socket
    *responder = zmq_socket(*context, ZMQ_REP);
    int rc = zmq_bind(*responder, TCP_PATH_REP);
    assert(rc == 0);

    // Initialize publisher socket
    *publisher = zmq_socket(*context, ZMQ_PUB);
    rc = zmq_bind(*publisher, TCP_PATH_PUB);
    assert(rc == 0);
}

/**
 * @brief Initialize the connection for the client
 *
 * @param context Pointer to the zmq context
 * @param requester Pointer to the zmq requester socket
 */
void initialize_connection_client(void **context, void **requester)
{
    *requester = zmq_socket(*context, ZMQ_REQ);
    zmq_connect(*requester, TCP_PATH_REQ);
}

/**
 * @brief Initialize the connection for the subscriber
 *
 * @param context Pointer to the zmq context
 * @param subscriber Pointer to the zmq subscriber socket
 * @param topic Topic to subscribe
 */
void initialize_connection_sub(void **context, void **subscriber, char *topic)
{
    *subscriber = zmq_socket(*context, ZMQ_SUB);
    zmq_connect(*subscriber, TCP_PATH_SUB);
    zmq_setsockopt(*subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));
}

/**
 * @brief Send message through TCP
 *
 * @param responder Pointer to the zmq responder socket
 * @param m message to send
 */
void send_TCP(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}


/**
 * @brief Receive message through TCP
 *
 * @param responder Pointer to the zmq responder socket
 * @param m message to receive
 */
void recv_TCP(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

/**
 * @brief Receive message to the subscribed topic through TCP
 *
 * @param subscriber Pointer to the zmq subscriber socket
 * @param all_data all ships data
 */
void recv_subscription_TCP(void *subscriber, all_ships_t *all_data)
{
    char topic[8];
    if (zmq_recv(subscriber, topic, 7, 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }

    if (zmq_recv(subscriber, all_data, sizeof(all_ships_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

/**
 * @brief Publish the display data to all outer-displays
 *
 * @param publisher Pointer to the zmq publisher socket
 * @param all_ships Pointer to the all_ships_t struct
 */
void publish_display_data(void *publisher, all_ships_t *all_ships)
{
    if (zmq_send(publisher, "Display", 7, ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    if (zmq_send(publisher, all_ships, sizeof(all_ships_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}
