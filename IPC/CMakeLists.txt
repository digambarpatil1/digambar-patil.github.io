cmake_minimum_required(VERSION 3.10)
project(IPC_Project C)

set(CMAKE_C_STANDARD 99)

# Add subdirectories for FIFO and Shared Memory
add_subdirectory(fifo)
add_subdirectory(sharedmemory)
