cmake_minimum_required(VERSION 3.10)
project(SharedMemory_IPC C)

set(CMAKE_C_STANDARD 11)

# Include the header directory
include_directories(${CMAKE_SOURCE_DIR}/sharedmemory/header)

# Define the shared library for shared memory utilities
add_library(shm_utils src/shm_utils.c)

# Create executables
add_executable(sharedmemory_producer src/shm_producer.c)
add_executable(sharedmemory_consumer src/shm_consumer.c)

# Link the shared library and necessary system libraries
target_link_libraries(sharedmemory_producer shm_utils pthread rt)
target_link_libraries(sharedmemory_consumer shm_utils pthread rt)
