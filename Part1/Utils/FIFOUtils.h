#ifndef FIFO_UTILS_H
#define FIFO_UTILS_H
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Server/space_aliens.h"

typedef struct alien_info_t alien_info_t;

int create_read_FIFO(char *);

int create_write_FIFO(char *);

void write_FIFO(int, alien_info_t *);

void recv_FIFO(int, alien_info_t *);

#define FIFO_LOCATION_CHILD_PARENT "/tmp/SPCInvdrsALIENS"

#define FIFO_LOCATION_PARENT_CHILD "/tmp/SPCInvdrsSERVER"

#endif