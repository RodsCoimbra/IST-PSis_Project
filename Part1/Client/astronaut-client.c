#include "astronaut-client.h"

int main()
{
    remote_char_t m;
    int disconnected = 0;
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, TCP_PATH_REQ);

    m.action = Astronaut_connect;

    if (zmq_send(requester, &m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }

    if (zmq_recv(requester, &m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }

    initialize_ncurses();

    do
    {
        disconnected = execute_action(&m);
        if (disconnected != 'x')
        {
            if (zmq_send(requester, &m, sizeof(remote_char_t), 0) == -1)
            {
                perror("zmq_send");
                exit(1);
            }
            if (zmq_recv(requester, &m, sizeof(remote_char_t), 0) == -1)
            {
                perror("zmq_recv");
                exit(1);
            }
            mvprintw(0, 0, "Ship %c with pontuation: %d", m.ship, m.points);
        }
        refresh(); /* Print it on to the real screen */
    } while (disconnected != 1);

    endwin(); /* End curses mode*/
    zmq_close(requester);
    zmq_ctx_destroy(context);

    printf("Disconnected ship %c\n", m.ship);
    printf("Final Pontuation: %d\n", m.points);
    return 0;
}

void initialize_ncurses()
{
    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
}

int execute_action(remote_char_t *m)
{
    int key;
    key = getch();
    switch (key)
    {
    case KEY_LEFT:
        m->direction = LEFT;
        break;
    case KEY_RIGHT:
        m->direction = RIGHT;
        break;
    case KEY_DOWN:
        m->direction = DOWN;
        break;
    case KEY_UP:
        m->direction = UP;
        break;
    case KEY_SPACE:
        m->action = Astronaut_zap;
        break;
    case KEY_q:
    case KEY_Q:
        m->action = Astronaut_disconnect;
        return 1;
        break;
    default:
        return 'x';
        break;
    }
    return 0;
}