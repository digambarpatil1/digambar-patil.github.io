cmake_minimum_required(VERSION 3.10)
project(linux LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Subdirectories
add_subdirectory(ipc)

# Executables
add_executable(deadlock deadlock.cpp)
add_executable(racecondition racecondition.cpp)
add_executable(fork_exec_demo fork_exec_demo.cpp)




