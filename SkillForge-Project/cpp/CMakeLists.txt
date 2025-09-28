cmake_minimum_required(VERSION 3.10)
project(C++ LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")


# Executables (one per file with main)
#add_executable(generic_feature src/generic_feature.cpp)
#add_executable(smart_pointers  src/smart_pointers.cpp)
#add_executable(move_semantic   src/move_semantic.cpp)
#add_executable(templates       src/templates.cpp)
#add_executable(lambdas       src/lambdas.cpp)
#add_executable(threads  src/threads.cpp)
#add_executable(threadpool  src/threadpool.cpp)
#add_executable(threadsync  src/threadsync.cpp)
#add_executable(producer_consumer  src/producer_consumer.cpp)
#add_executable(temp  src/temp.cpp)
#add_executable(inheritance  src/inheritance.cpp)
#add_executable(polymorphism  src/polymorphism.cpp)
#add_executable(memory_optimization  src/memory_optimization.cpp)
#add_executable(ranges  src/ranges.cpp)
add_executable(casting_operator  src/casting_operator.cpp)


