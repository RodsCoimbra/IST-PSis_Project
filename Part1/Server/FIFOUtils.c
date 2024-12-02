#include "FIFOUtils.h"

int read_FIFO(char *FIFO_LOCATION)
{
    int fd;
    while ((fd = open(FIFO_LOCATION, O_RDONLY)) == -1)
    {
        if (mkfifo(FIFO_LOCATION, 0666) != 0)
        {
            perror("problem creating the fifo");
            exit(-1);
        }
    }
    return fd;
}

int write_FIFO(char *FIFO_LOCATION)
{
    int fd;
    while ((fd = open(FIFO_LOCATION, O_WRONLY)) == -1)
    {
        if (mkfifo(FIFO_LOCATION, 0666) != 0)
        {
            perror("problem creating the fifo");
            exit(-1);
        }
    }
    return fd;
}

void write_msg(int fd, alien_info_t *m)
{
    if (write(fd, m, sizeof(alien_info_t)) <= 0)
    {
        perror("Error writing to FIFO");
        exit(-1);
    }
}

void receive_msg(int fd, alien_info_t *m)
{
    if (read(fd, m, sizeof(alien_info_t)) <= 0)
    {
        perror("Error reading from FIFO");
        exit(-1);
    }
}