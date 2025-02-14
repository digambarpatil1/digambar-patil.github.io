#ifndef FIFO_PRODUCER_H
#define FIFO_PRODUCER_H

#define FIFO_PATH "/tmp/myfifo"

// Function to create FIFO and write message
void create_and_write_fifo(const char *message);

#endif // FIFO_PRODUCER_H
