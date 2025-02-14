cmake_minimum_required(VERSION 3.10)
project(FIFO_IPC C)

# Include the header directory
include_directories(${CMAKE_SOURCE_DIR}/fifo/header)

# Source files
set(FIFO_SOURCES
    src/fifoConsumer.c
    src/fifoProducer.c
)

# Create executables
add_executable(fifo_producer src/fifoProducer.c)
add_executable(fifo_consumer src/fifoConsumer.c)

# Link any necessary libraries (if needed)
#target_link_libraries(fifo_producer)
#target_link_libraries(fifo_consumer)
