#ifndef REMOTE_CHAR_H
#define REMOTE_CHAR_H

#include <time.h>

#define WINDOW_SIZE 22
#define MAX_POS WINDOW_SIZE - 4
#define MIN_POS 3
#define MID_POS WINDOW_SIZE / 2

#define N_SHIPS 8
#define N_ALIENS ((WINDOW_SIZE - 2) * (WINDOW_SIZE - 2) / 3)
#define RECHARGING_TIME 3
#define STUN_TIME 10

#define TCP_PATH_REP "tcp://*:5505"
#define TCP_PATH_PUB "tcp://*:5506"
#define TCP_PATH_REQ "tcp://localhost:5505"
#define TCP_PATH_SUB "tcp://localhost:5506"
#define KEY_SPACE 32
#define KEY_q 113
#define KEY_Q 81

typedef enum movement_t
{
    VERTICAL,
    HORIZONTAL
} movement_t;

typedef enum direction_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef enum zap_display_t
{
    NO_ZAP,
    DRAW_ZAP,
    ERASE_ZAP

} zap_display_t;

typedef enum msg_type_t
{
    Astronaut_connect,
    Astronaut_movement,
    Astronaut_zap,
    Astronaut_disconnect,
    Alien_movement,
    Alien_end
} msg_type_t;

typedef struct remote_char_t
{
    char ship;
    int points;
    msg_type_t action;
    direction_t direction;
    int encryption;
} remote_char_t;

typedef struct position_info_t
{
    int x, y;
} position_info_t;

typedef struct ship_info_t
{
    int ship;
    position_info_t position;
    int points;
    movement_t move_type;
    time_t timeouts[2];
    zap_display_t zap;
    int encryption;
} ship_info_t;

typedef struct alien_info_t
{
    position_info_t position;
    int alive;
    int encryption;
} alien_info_t;

typedef enum status_t
{
    RECHARGING,
    STUNNED
} status_t;

typedef struct all_ships_t
{
    ship_info_t ships[N_SHIPS];
    alien_info_t aliens[N_ALIENS];
} all_ships_t;

#endif