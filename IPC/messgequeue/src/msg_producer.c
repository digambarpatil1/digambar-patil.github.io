// producer.c
#include <stdio.h>
#include <string.h>
#include "message_queue.h"

int main() {
    key_t key = ftok("progfile", 65);  // Generate a unique key
    int msgid = create_or_open_message_queue(key);  // Create message queue

    struct msg_buffer message;
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello from producer!");

    send_message(msgid, &message);  // Send the message
    printf("Message sent: %s\n", message.msg_text);

    return 0;
}
