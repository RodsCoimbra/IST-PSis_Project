#include "communication.h"

/**
 * @brief Initialize the connection for the server
 *
 * @param context Pointer to the zmq context
 * @param responder Pointer to the zmq responder socket
 * @param publisher Pointer to the zmq publisher socket
 */
void initialize_connection_server(void **context, void **responder, void **publisher)
{
    // Create new zmq context
    *context = zmq_ctx_new();

    // Initialize responder socket
    *responder = zmq_socket(*context, ZMQ_REP);
    int rc = zmq_bind(*responder, TCP_PATH_REP);
    assert(rc == 0);

    // Initialize publisher socket
    *publisher = zmq_socket(*context, ZMQ_PUB);
    rc = zmq_bind(*publisher, TCP_PATH_PUB);
    assert(rc == 0);
}

/**
 * @brief Initialize the connection for the client
 *
 * @param context Pointer to the zmq context
 * @param requester Pointer to the zmq requester socket
 */
void initialize_connection_client(void **context, void **requester)
{
    *requester = zmq_socket(*context, ZMQ_REQ);
    zmq_connect(*requester, TCP_PATH_REQ);
}

/**
 * @brief Initialize the connection for the subscriber
 *
 * @param context Pointer to the zmq context
 * @param subscriber Pointer to the zmq subscriber socket
 * @param topic Topic to subscribe
 */
void initialize_connection_sub(void **context, void **subscriber)
{
    char topic[] = "Display", topic2[] = "Disconn";
    *subscriber = zmq_socket(*context, ZMQ_SUB);
    zmq_connect(*subscriber, TCP_PATH_SUB);
    zmq_setsockopt(*subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));
    zmq_setsockopt(*subscriber, ZMQ_SUBSCRIBE, topic2, strlen(topic2));
}

/**
 * @brief Send message through TCP
 *
 * @param responder Pointer to the zmq responder socket
 * @param m message to send
 */
void send_TCP(void *responder, remote_char_t *m)
{
    if (zmq_send(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
}

/**
 * @brief Receive message through TCP
 *
 * @param responder Pointer to the zmq responder socket
 * @param m message to receive
 */
void recv_TCP(void *responder, remote_char_t *m)
{
    if (zmq_recv(responder, m, sizeof(remote_char_t), 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }
}

/**
 * @brief Receive message to the subscribed topic through TCP
 *
 * @param subscriber Pointer to the zmq subscriber socket
 * @param all_data all ships data
 */
void recv_subscription_TCP(void *subscriber, all_ships_t *all_data, long int *game_end)
{
    char topic[8];
    if (zmq_recv(subscriber, topic, 7, 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }

    size_t msg_size = msg_ships_size + msg_aliens_size;
    char *buffer = malloc(msg_size);

    if (zmq_recv(subscriber, buffer, msg_size, 0) == -1)
    {
        perror("zmq_recv");
        exit(1);
    }

    if (strcmp(topic, "Display") == 0)
    {
        memcpy(all_data->ships, buffer, msg_ships_size);
        memcpy(all_data->aliens, buffer + msg_ships_size, msg_aliens_size);
    }
    else if (strcmp(topic, "Disconn") == 0)
    {
        *game_end = 1;
    }
    free(buffer);
}

/**
 * @brief Publish the display data to all outer-displays
 *
 * @param publisher Pointer to the zmq publisher socket
 * @param all_ships Pointer to the all_ships_t struct
 */
void publish_display_data(void *publisher, all_ships_t *all_ships)
{
    if (zmq_send(publisher, "Display", 8, ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    size_t msg_size = msg_ships_size + msg_aliens_size;
    char *buffer = malloc(msg_size);
    memcpy(buffer, all_ships->ships, msg_ships_size);
    memcpy(buffer + msg_ships_size, all_ships->aliens, msg_aliens_size);

    if (zmq_send(publisher, buffer, msg_size, 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }

    free(buffer);

    send_scoreboard(publisher, all_ships);
}

/**
 * @brief Publish the end game message to all displays
 *
 * @param publisher Pointer to the zmq publisher socket
 */
void publish_end_game(void *publisher)
{
    if (zmq_send(publisher, "Disconn", 8, ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    size_t msg_size = msg_ships_size + msg_aliens_size;
    char *buffer = malloc(msg_size);

    if (zmq_send(publisher, buffer, msg_size, 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }
    free(buffer);
}

/**
 * @brief Send the scoreboard to all displays
 *
 * @param publisher Pointer to the zmq publisher socket
 * @param all_ships Pointer to the all_ships_t struct
 */
void send_scoreboard(void *publisher, const all_ships_t *all_ships)
{
    ScoreBoard scoreboard = SCORE_BOARD__INIT;
    scoreboard.scores = malloc(sizeof(ScoreUpdate *) * N_SHIPS);
    scoreboard.n_scores = 0;

    for (int i = 0; i < N_SHIPS; i++)
    {
        if (all_ships->ships[i].ship == 0)
            continue;

        scoreboard.scores[scoreboard.n_scores] = malloc(sizeof(ScoreUpdate));
        score_update__init(scoreboard.scores[scoreboard.n_scores]);

        // Copy data from all_ships
        scoreboard.scores[scoreboard.n_scores]->astronaut = all_ships->ships[i].ship;
        scoreboard.scores[scoreboard.n_scores]->points = all_ships->ships[i].points;
        scoreboard.n_scores++;
    }

    // Pack once
    size_t msg_len = score_board__get_packed_size(&scoreboard);
    uint8_t *msg_buf = malloc(msg_len);
    score_board__pack(&scoreboard, msg_buf);

    if (zmq_send(publisher, "Score", 5, ZMQ_SNDMORE) == -1)
    {
        perror("zmq_send");
        exit(1);
    }

    if (zmq_send(publisher, msg_buf, msg_len, 0) == -1)
    {
        perror("zmq_send");
        exit(1);
    }

    // Cleanup
    free(msg_buf);
    for (size_t i = 0; i < scoreboard.n_scores; i++)
        free(scoreboard.scores[i]);

    free(scoreboard.scores);
}