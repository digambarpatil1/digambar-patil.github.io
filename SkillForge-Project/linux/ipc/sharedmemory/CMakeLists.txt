cmake_minimum_required(VERSION 3.10)
project(SharedMemory_IPC C)

set(CMAKE_C_STANDARD 11)

add_library(shared_memory STATIC src/shm_utils.c)
target_include_directories(shared_memory PUBLIC header)
