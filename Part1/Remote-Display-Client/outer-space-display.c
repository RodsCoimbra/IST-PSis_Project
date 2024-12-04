#include "outer-space-display.h"

int main()
{
    void *context, *subscriber;
    WINDOW *space;

    initialize_connection_sub(&context, &subscriber, "Display");

    initialize_window(&space);

    ship_info_t ships[N_SHIPS];
    alien_info_t aliens[N_ALIENS];

    while (1)
    {
        erase_old_data(space, ships, aliens);

        receive_ships(subscriber, ships, aliens);

        display_new_data(space, ships, aliens);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);
    return 0;
}

void receive_ships(void *subscriber, ship_info_t *ships, alien_info_t *alien)
{
    recv(subscriber, type)
}

void erase_old_data(WINDOW *space, ship_info_t *ships, alien_info_t *aliens)
{
    // Erase ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        wmove(space, ships[i].position.x, ships[i].position.y);
        waddch(space, ' ');
    }

    // Erase aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        wmove(space, aliens[i].position.x, aliens[i].position.y);
        waddch(space, ' ');
    }

    wrefresh(space);
}

void display_new_data(WINDOW *space, ship_info_t *ships, alien_info_t *aliens)
{
    // Display ships
    for (int i = 0; i < N_SHIPS; i++)
    {
        wmove(space, ships[i].position.x, ships[i].position.y);
        waddch(space, ships[i].ship);
    }

    // Display aliens
    for (int i = 0; i < N_ALIENS; i++)
    {
        wmove(space, aliens[i].position.x, aliens[i].position.y);
        waddch(space, aliens[i].alien);
    }

    wrefresh(space);
}