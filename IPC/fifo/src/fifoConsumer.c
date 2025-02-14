#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "fifo_consumer.h"

// Function to read from FIFO
void read_from_fifo(void) {
    // Check if the FIFO exists
    if (access(FIFO_PATH, F_OK) == -1) {
        perror("FIFO does not exist");
        exit(EXIT_FAILURE);
    }

    // Open FIFO for reading
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);  // Leave space for null terminator
    if (bytesRead >= 0) {
        buffer[bytesRead] = '\0';  // Ensure null termination
        printf("Consumer received: %s\n", buffer);
    } else {
        perror("read");
    }

    // Close FIFO
    close(fd);
}

int main() {
    // Call function to read from FIFO
    read_from_fifo();

    return 0;
}
