#include <ncurses.h>
#include <zmq.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include "../Utils/remote-char.h"
#include "../Utils/communication.h"
#include "../Utils/Utils.h"
#include "astronaut-client.h"