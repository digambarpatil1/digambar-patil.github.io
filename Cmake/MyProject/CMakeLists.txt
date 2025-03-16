cmake_minimum_required(VERSION 3.16)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set the output directory for executables to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/FetchDependencies.cmake)

add_subdirectory(src)
add_subdirectory(tests)

install(TARGETS my_app RUNTIME DESTINATION bin)
install(TARGETS my_lib ARCHIVE DESTINATION lib)

