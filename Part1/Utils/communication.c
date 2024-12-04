#include "communication.h"

void initialize_connection_server(void **context, void **responder, void **publisher)
{
    *context = zmq_ctx_new();
    // Responder
    *responder = zmq_socket(*context, ZMQ_REP);
    int rc = zmq_bind(*responder, TCP_PATH_REP);
    assert(rc == 0);
    // Publisher
    *publisher = zmq_socket(*context, ZMQ_PUB);
    rc = zmq_bind(*publisher, TCP_PATH_PUB);
    assert(rc == 0);
}

void initialize_connection_client(void **context, void **requester)
{
    *context = zmq_ctx_new();
    *requester = zmq_socket(*context, ZMQ_REQ);
    zmq_connect(*requester, TCP_PATH_REQ);
}

void initialize_connection_sub(void **context, void **subscriber, char *topic)
{
    *context = zmq_ctx_new();
    *subscriber = zmq_socket(*context, ZMQ_SUB);
    zmq_connect(*subscriber, TCP_PATH_SUB);
    zmq_setsockopt(*subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));
}

void send_TCP(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}

void recv_TCP(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

void recv_subscription_TCP(void *subscriber, all_ships_t *all_data)
{
    char type[20];
    if (zmq_recv(subscriber, type, 20, 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }

    if (zmq_recv(subscriber, all_data, sizeof(all_data), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

void publish_display_data(void *publisher, all_ships_t *all_ships, char *topic)
{
    if (zmq_send(publisher, topic, 20, ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    if (zmq_send(publisher, all_ships, sizeof(all_ships), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}
