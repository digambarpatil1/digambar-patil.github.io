#include <stdio.h>
#include <sys/wait.h>
#include "fifo.h"
#include "shm_utils.h"
#include "message_queue.h"

int main() {

    printf("IPC Demo Application\n");
    
    //unmaed pipe
    int fd[2];
    pipe(fd);// fd[0] for read ,    fd[1] for write
    if (fork() == 0) {
        // Child process: Writer
        close(fd[0]); // Close unused read end
        const char *message = "Hello from unamed pipe!";
        write(fd[1], message, strlen(message));
        close(fd[1]); // Close write end
        return 0;
    } else {
        // Parent process: Reader
        close(fd[1]); // Close unused write end
        char buf[100];
        int n = read(fd[0], buf, sizeof(buf) - 1); // leave space for '\0'
        buf[n] = '\0';  // Null-terminate
        printf("Parent received from unamed pipe: %s\n", buf);
       
        close(fd[0]); // Close read end
        wait(NULL); // Wait for child process to finish
    }   
    // Named Pipe (FIFO) Example 
    // At the start of main()
    unlink(FIFO_PATH);               // remove old FIFO
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();  // Create a child process
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: Consumer
        read_from_fifo();
        exit(0);
    } else {
        // Parent process: Producer
        sleep(1);  // Give consumer time to start
        const char *message = "Hello from FIFO!";
        create_and_write_fifo(message);

        // Wait for child process to finish
        wait(NULL);

        // Remove FIFO file after usage
        unlink(FIFO_PATH);
    }


    // Shared Memory Example
    create_shared_memory();
    char shm_message[] = "Hello from Shared Memory!";
    write_shared_memory(shm_message);
    read_shared_memory();
    cleanup_shared_memory();


    // Message Queue Example
    key_t key = ftok("progfile", 65);  // Generate a unique key
    int msgid = create_or_open_message_queue(key);  // Create message queue     
    struct msg_buffer message;
    message.msg_type = 1;
    snprintf(message.msg_text, sizeof(message.msg_text), "Hello from Message Queue!");
    send_message(msgid, &message);  // Send the message
    printf("Message sent: %s\n", message.msg_text);
    struct msg_buffer received_message; 
    receive_message(msgid, &received_message);  // Receive the message
    printf("Message received: %s\n", received_message.msg_text);
    remove_message_queue(msgid);  // Remove the message queue
    
    return 0;
}