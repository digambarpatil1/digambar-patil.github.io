#ifndef FIFO_CONSUMER_H
#define FIFO_CONSUMER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/myfifo"  // FIFO path

// Function to read from FIFO
void read_from_fifo(void);

// Function to create FIFO and write message
void create_and_write_fifo(const char *message);

#endif // FIFO_CONSUMER_H

