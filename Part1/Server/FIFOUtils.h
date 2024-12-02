#ifndef FIFO_UTILS_H
#define FIFO_UTILS_H
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "game-server.h"

typedef struct alien_info_t alien_info_t;

int read_FIFO(char *);

int write_FIFO(char *);

void write_msg(int fd, alien_info_t *m);

void receive_msg(int fd, alien_info_t *m);

#define FIFO_LOCATION_CHILD_PARENT "/tmp/SPCInvdrsALIENS"

#define FIFO_LOCATION_PARENT_CHILD "/tmp/SPCInvdrsSERVER"

#endif