cmake_minimum_required(VERSION 3.10)
project(MessageQueue_IPC C)

# Include the header directory
include_directories(${CMAKE_SOURCE_DIR}/messgequeue/header)


# Define the shared library for shared memory utilities
add_library(message_queue src/message_queue.c)

# Create executables
add_executable(messgequeue_producer src/msg_producer.c)
add_executable(messgequeue_consumer src/msg_consumer.c)

# Link the shared library and necessary system libraries
target_link_libraries(messgequeue_producer message_queue pthread rt)
target_link_libraries(messgequeue_consumer message_queue pthread rt)


