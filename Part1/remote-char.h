#ifndef REMOTE_CHAR_H
#define REMOTE_CHAR_H

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

typedef enum msg_type_t
{
    Astronaut_connect,
    Astronaut_movement,
    Astronaut_zap,
    Astronaut_disconnect
} msg_type_t;

typedef struct remote_char_t
{
    char ship;
    int points;
    msg_type_t action;
    direction_t direction;
    /* data */
} remote_char_t;

#define TCP_PATH_REP "tcp://*:5555"
#define TCP_PATH_PUB "tcp://*:5556"
#define TCP_PATH_REQ "tcp://localhost:5555"
#define TCP_PATH_SUB "tcp://localhost:5556"
#define KEY_SPACE 32
#define KEY_q 113
#define KEY_Q 81

#endif