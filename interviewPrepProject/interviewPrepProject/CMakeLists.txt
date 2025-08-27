cmake_minimum_required(VERSION 3.10)
project(interviewPrepProject LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")


# Executables (one per file with main)
add_executable(generic_feature src/generic_feature.cpp)
add_executable(smart_pointers  src/smart_pointers.cpp)
add_executable(move_semantic   src/move_semantic.cpp)
add_executable(templates       src/templates.cpp)
add_executable(lambdas       src/lambdas.cpp)
add_executable(threads  src/threads.cpp)
add_executable(threadpool  src/threadpool.cpp)
add_executable(threadsync  src/threadsync.cpp)
add_executable(producer_consumer  src/producer_consumer.cpp)
add_executable(temp  src/temp.cpp)
