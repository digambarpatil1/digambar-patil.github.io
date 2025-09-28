cmake_minimum_required(VERSION 3.10)
project(interviewPrepProject LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")


# Executables (one per file with main)
#add_executable(frequncyCount frequncyCount.cpp)
#add_executable(lederboard leaderboard.cpp)
#add_executable(dqueue dqueue.cpp)
#add_executable(memoryleakdetection memoryleakdetection.cpp)
#add_executable(reverselinkedlist reverselinkedlist.cpp)
add_executable(nonrepeatingcharacterstream nonrepeatingcharacterstream.cpp)


#add_executable(LRUCache LRUCache.cpp)
#add_executable(LRUCache LRUCache.cpp)
