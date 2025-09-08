// message_queue.c
#include "message_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create or open an existing message queue
int create_or_open_message_queue(key_t key) {
    int msgid = msgget(key, 0666 | IPC_CREAT);  // Create or open the message queue
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    return msgid;
}

// Function to send a message
int send_message(int msgid, struct msg_buffer *message) {
    if (msgsnd(msgid, message, sizeof(*message), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }
    return 0;
}

// Function to receive a message
int receive_message(int msgid, struct msg_buffer *message) {
    if (msgrcv(msgid, message, sizeof(*message), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }
    return 0;
}

// Function to remove the message queue
int remove_message_queue(int msgid) {
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }
    return 0;
}
