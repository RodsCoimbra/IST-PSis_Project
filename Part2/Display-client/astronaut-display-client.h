#ifndef ASTRONAUT_DISPLAY_CLIENT_H
#define ASTRONAUT_DISPLAY_CLIENT_H

#include <pthread.h>
#include "../Server/space_ship.h"
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "outer-space-display.h"
#include "astronaut-client.h"

// TODO Isto é inutil???? Precisamos sequer de um .h??
void *disconnect_server(void *);

#endif