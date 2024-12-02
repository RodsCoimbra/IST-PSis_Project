#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <zmq.h>
#include <assert.h>
#include <stdlib.h>
#include "remote-char.h"

void initialize_connection_server(void **, void **, void **);

void initialize_connection_client(void **, void **);

void send_TCP(void *, remote_char_t *);

void recv_TCP(void *, remote_char_t *);

// void publish_display_data(void *publisher, ship_info_t *ship_data, char *topic);

#endif