#include "astronaut-client.h"

/**
 * @brief Client of the game, receives commands for movement and zapping of a single astronaut and sends it to the server
 *
 * @return Returns 0 if the program ends successfully.
 */
int main()
{
    client(NULL);
    return 0;
}

void *client(void *arg)
{
    remote_char_t m;
    int valid_action;

    void *context, *requester;
    initialize_connection_client(&context, &requester);

    m.action = Astronaut_connect;
    // first connection to receive encryption key and ship character
    send_TCP(requester, &m);

    recv_TCP(requester, &m);
    if (m.ship == 0)
    {
        printf("Server is full\n");
        return NULL;
    }

    initialize_ncurses();
    do
    {
        valid_action = execute_action(&m);
        if (valid_action)
        {
            send_TCP(requester, &m);
            recv_TCP(requester, &m);
            mvprintw(0, 0, "Ship %c with pontuation: %d", m.ship, m.points);
        }
        refresh(); /* Print it on to the real screen */
    } while (m.action != Astronaut_disconnect);

    endwin(); /* End curses mode*/
    zmq_close(requester);
    zmq_ctx_destroy(context);

    printf("Disconnected ship %c\n", m.ship);
    printf("Final Pontuation: %d\n", m.points);
    return NULL;
}

/**
 * @brief Receives the key pressed by the user and sets the action of the astronaut.
 *
 * @param m Holds the astronaut message for the server.
 *
 * @return Returns 1 if the key pressed is valid, 0 otherwise.
 */
int execute_action(remote_char_t *m)
{
    int key;
    key = getch();
    switch (key)
    {
    case KEY_LEFT:
        m->direction = LEFT;
        m->action = Astronaut_movement;
        break;
    case KEY_RIGHT:
        m->direction = RIGHT;
        m->action = Astronaut_movement;
        break;
    case KEY_DOWN:
        m->direction = DOWN;
        m->action = Astronaut_movement;
        break;
    case KEY_UP:
        m->direction = UP;
        m->action = Astronaut_movement;
        break;
    case KEY_SPACE:
        m->action = Astronaut_zap;
        break;
    case KEY_q:
    case KEY_Q:
        m->action = Astronaut_disconnect;
        break;
    default:
        return 0;
        break;
    }
    return 1;
}