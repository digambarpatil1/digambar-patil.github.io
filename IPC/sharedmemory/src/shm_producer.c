#include "shm_utils.h"

int main() {
    create_shared_memory();
    char message[] = "Hello from Producer!";
    write_shared_memory(message);
    return 0;
}
