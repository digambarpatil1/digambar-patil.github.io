// message_queue.h
#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

// Function to create or open an existing message queue
int create_or_open_message_queue(key_t key);

// Function to send a message
int send_message(int msgid, struct msg_buffer *message);

// Function to receive a message
int receive_message(int msgid, struct msg_buffer *message);

// Function to remove the message queue
int remove_message_queue(int msgid);

#endif // MESSAGE_QUEUE_H
