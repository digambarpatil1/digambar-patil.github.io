#!/bin/bash

set -e  # Exit on any error

# Project root
PROJECT_NAME="MyProject"
ROOT_DIR="$(pwd)/$PROJECT_NAME"

# Define folders
DIRS=(
    "$ROOT_DIR/cmake"
    "$ROOT_DIR/src"
    "$ROOT_DIR/include"
    "$ROOT_DIR/external"
    "$ROOT_DIR/build"
    "$ROOT_DIR/tests"
    "$ROOT_DIR/toolchains"
    "$ROOT_DIR/package"
)

# Create directories
echo "Creating project structure..."
for dir in "${DIRS[@]}"; do
    mkdir -p "$dir"
done

# Create CMakeLists.txt (Root)
cat > "$ROOT_DIR/CMakeLists.txt" <<EOL
cmake_minimum_required(VERSION 3.16)
project($PROJECT_NAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/FetchDependencies.cmake)

add_subdirectory(src)
add_subdirectory(tests)

install(TARGETS my_app RUNTIME DESTINATION bin)
install(TARGETS my_lib ARCHIVE DESTINATION lib)
EOL

# Create FetchDependencies.cmake
cat > "$ROOT_DIR/cmake/FetchDependencies.cmake" <<EOL
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.0
)

FetchContent_MakeAvailable(fmt)
EOL

# Create CMakeLists.txt (src)
cat > "$ROOT_DIR/src/CMakeLists.txt" <<EOL
add_library(my_lib STATIC lib.cpp)
target_include_directories(my_lib PUBLIC \${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(my_lib PUBLIC fmt)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE my_lib)
EOL

# Create Sample C++ Source Files
cat > "$ROOT_DIR/src/main.cpp" <<EOL
#include <iostream>
#include "lib.h"
#include <fmt/core.h>

int main() {
    fmt::print("Hello, CMake Project!\\n");
    print_message();
    return 0;
}
EOL

cat > "$ROOT_DIR/src/lib.cpp" <<EOL
#include "lib.h"
#include <iostream>

void print_message() {
    std::cout << "Library function called!\\n";
}
EOL

# Create Sample Header File
cat > "$ROOT_DIR/include/lib.h" <<EOL
#ifndef LIB_H
#define LIB_H

void print_message();

#endif // LIB_H
EOL

# Create CMakeLists.txt (tests)
cat > "$ROOT_DIR/tests/CMakeLists.txt" <<EOL
enable_testing()

add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE my_lib)
add_test(NAME MyTest COMMAND test_main)
EOL

# Create Sample Test File
cat > "$ROOT_DIR/tests/test_main.cpp" <<EOL
#include "lib.h"
#include <iostream>

int main() {
    print_message();
    std::cout << "Test executed!\\n";
    return 0;
}
EOL

# Create CMakePresets.json
cat > "$ROOT_DIR/CMakePresets.json" <<EOL
{
  "version": 3,
  "cmakeMinimumRequired": { "major": 3, "minor": 20 },
  "configurePresets": [
    {
      "name": "debug",
      "generator": "Ninja",
      "binaryDir": "\${sourceDir}/build/debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    },
    {
      "name": "release",
      "generator": "Ninja",
      "binaryDir": "\${sourceDir}/build/release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ]
}
EOL

# Create toolchain file (ARM Cross-compilation)
cat > "$ROOT_DIR/toolchains/arm-toolchain.cmake" <<EOL
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
EOL

# Set executable permission
chmod +x "$ROOT_DIR/setup_project.sh"

echo "Project structure for '$PROJECT_NAME' created successfully!"

