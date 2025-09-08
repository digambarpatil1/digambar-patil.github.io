#ifndef SHM_UTILS_H
#define SHM_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME "/my_semaphore"
#define MAX_SIZE 256

// Function declarations
void create_shared_memory();
void write_shared_memory(const char *message);
char* read_shared_memory();
void cleanup_shared_memory();

#endif  // SHM_UTILS_H
