#include <ncurses.h>
#include "../remote-char.h"
#include <zmq.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>

void initialize_ncurses();

int execute_action(remote_char_t *m);

void send_msg(void *requester, remote_char_t *m);

void recv_msg(void *requester, remote_char_t *m);