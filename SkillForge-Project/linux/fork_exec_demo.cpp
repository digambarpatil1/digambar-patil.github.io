#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int x = 42;  // original stack variable
    printf("Parent PID: %d, x=%d\n", getpid(), x);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } 

    if (pid == 0) {
        // Child process
        int y = 100;  // variable on child stack
        printf("[Child] Before exec, y=%d &y=%n\n", y,&y);

        // Replace child process with a new program
       execlp("ls", "ls", "-l", NULL);


        // This runs only if exec fails
        printf("[Child] After exec &y=%n (should never print)\n",&y);
        // If exec fails
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        printf("[Parent] Child finished\n");
    }
    return 0;
}
