cmake_minimum_required(VERSION 3.10)
project(IPC_Project C)

set(CMAKE_C_STANDARD 99)

# Add subdirectories for FIFO and Shared Memory
add_subdirectory(fifo)
add_subdirectory(sharedmemory)
add_subdirectory(messgequeue)

# Create executable
add_executable(main main.c)

# Link libraries
target_link_libraries(main PRIVATE fifo shared_memory messgequeue -lpthread rt)

target_include_directories( main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/fifo/header
    ${CMAKE_CURRENT_SOURCE_DIR}/sharedmemory/header
    ${CMAKE_CURRENT_SOURCE_DIR}/messgequeue/header
)