#include "communication.h"

/*
 * Function: initialize_connection_server
 *
 * Initialize connection for server
 * @param context: zmq context
 * @param responder: zmq socket
 * @param publisher: zmq socket
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

/*
 * Function: initialize_connection_client
 *
 * Initialize connection for client
 * @param context: zmq context
 * @param requester: zmq socket
 */
void initialize_connection_client(void **context, void **requester)
{
    *context = zmq_ctx_new();
    *requester = zmq_socket(*context, ZMQ_REQ);
    zmq_connect(*requester, TCP_PATH_REQ);
}

/*
 * Function: initialize_connection_sub
 *
 * Initialize connection for subscriber
 * @param context: zmq context
 * @param subscriber: zmq socket
 * @param topic: topic to subscribe
 */
void initialize_connection_sub(void **context, void **subscriber, char *topic)
{
    *context = zmq_ctx_new();
    *subscriber = zmq_socket(*context, ZMQ_SUB);
    zmq_connect(*subscriber, TCP_PATH_SUB);
    zmq_setsockopt(*subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));
}

/*
 * Function: send_TCP
 *
 * Send message through TCP
 * @param responder: zmq socket
 * @param m: message to send
 */
void send_TCP(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}

/*
 * Function: recv_TCP
 *
 * receive message through TCP
 * @param subscriber: zmq socket
 * @param topic: topic to subscribe
 */
void recv_TCP(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

/*
 * Function: send_subscription_TCP
 *
 * Send subscription message through TCP
 * @param subscriber: zmq socket
 * @param all_data: all data to send
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

/*
 * Function: publish_display_data
 *
 * Publish display data to all outer displays
 * @param publisher: zmq socket
 * @param all_ships: all ships data
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
