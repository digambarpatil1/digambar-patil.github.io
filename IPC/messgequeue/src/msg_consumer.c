// consumer.c
#include <stdio.h>
#include "message_queue.h"

int main() {
    key_t key = ftok("progfile", 65);  // Generate a unique key
    int msgid = create_or_open_message_queue(key);  // Create message queue

    struct msg_buffer message;
    receive_message(msgid, &message);  // Receive the message
    printf("Message received: %s\n", message.msg_text);

    remove_message_queue(msgid);  // Remove the message queue
    return 0;
}
