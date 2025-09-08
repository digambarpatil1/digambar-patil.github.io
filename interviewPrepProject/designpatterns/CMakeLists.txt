cmake_minimum_required(VERSION 3.10)
project(designpattern LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")


# Executables (one per file with main)
add_executable(singleton singleton.cpp)
add_executable(observer observer.cpp)
add_executable(strategy strategy.cpp)



