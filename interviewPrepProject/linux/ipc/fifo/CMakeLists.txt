cmake_minimum_required(VERSION 3.10)
project(FIFO_IPC C)


add_library(fifo STATIC src/fifo.c)
target_include_directories(fifo PUBLIC header)