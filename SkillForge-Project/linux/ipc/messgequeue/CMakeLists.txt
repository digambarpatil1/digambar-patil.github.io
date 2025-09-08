cmake_minimum_required(VERSION 3.10)
project(MessageQueue_IPC C)

add_library(messgequeue STATIC src/message_queue.c)
target_include_directories(messgequeue PUBLIC header)


