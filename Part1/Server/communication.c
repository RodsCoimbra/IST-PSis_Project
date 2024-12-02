#include "communication.h"

void initialize_connection(void **context, void **responder, void **publisher)
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

void send_msg(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}

void recv_msg(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

/*
void publish_display_data(void *publisher, ship_info_t *ship_data, char *topic)
{
    if (zmq_send(publisher, topic, strlen(topic), ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    if (zmq_send(publisher, ship_data, sizeof(ship_info_t) * N_SHIPS, 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}
*/
