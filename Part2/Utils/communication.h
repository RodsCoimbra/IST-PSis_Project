#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <zmq.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "remote-char.h"
#include "../space.pb-c.h"

typedef struct all_ships_t all_ships_t;

void initialize_connection_server(void **, void **, void **);

void initialize_connection_client(void **, void **);

void initialize_connection_sub(void **, void **, char *);

void recv_subscription_TCP(void *, all_ships_t *);

void send_TCP(void *, remote_char_t *);

void recv_TCP(void *, remote_char_t *);

void publish_display_data(void *publisher, all_ships_t *all_ships);

void send_scoreboard(void *publisher, const all_ships_t *all_ships);

#endif