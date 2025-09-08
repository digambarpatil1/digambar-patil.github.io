#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "fifo.h"

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

// Function to create FIFO and write message
void create_and_write_fifo(const char *message) {
    
     // Check if FIFO already exists
     if (access(FIFO_PATH, F_OK) == -1) {
        // FIFO does not exist, so create it
        if (mkfifo(FIFO_PATH, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    // Open FIFO for writing
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Calculate message length, excluding the null terminator
    size_t message_len = strlen(message);

    // Write data to FIFO
    ssize_t bytes_written = write(fd, message, message_len);
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("Producer sent: %s\n", message);

    // Close and cleanup
    close(fd);
}
