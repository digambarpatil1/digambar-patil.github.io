#!/bin/bash

# Set the project root directory
PROJECT_ROOT="$HOME/IPC"
BUILD_DIR="$PROJECT_ROOT/build"

# Ensure we are in the project directory
cd "$PROJECT_ROOT" || { echo "Error: Cannot change directory to $PROJECT_ROOT"; exit 1; }

# Function to build the project
build() {
    # Create or clean the build directory
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi

    cd "$BUILD_DIR" || { echo "Error: Cannot change to build directory"; exit 1; }

    # Run CMake and Make
    if ! command -v cmake &> /dev/null; then
        echo "Error: CMake not found. Install it first."
        exit 1
    fi

    cmake ..
    make
}

# Function to clean the build directory
clean() {
    if [ -d "$BUILD_DIR" ]; then
        echo "Cleaning up build directory..."
        rm -rf "$BUILD_DIR"
        echo "Build directory removed."
    else
        echo "No build directory found to clean."
    fi
}

# Function to run specific IPC module
run_module() {
    local module="$1"
    local producer="$BUILD_DIR/${module}/${module}_producer"
    local consumer="$BUILD_DIR/${module}/${module}_consumer"

    if [ ! -f "$producer" ] || [ ! -f "$consumer" ]; then
        echo "Error: Executables for $module not found. Build the project first."
        exit 1
    fi

    echo "Starting $module IPC..."
    "$producer" &
    sleep 1
    "$consumer"
}

# Function to list available modules
list_modules() {
    echo "Available IPC modules:"
    ls "$PROJECT_ROOT" | grep -E 'fifo|sharedmemory|messagequeue'  # Add more modules as needed
}

# Parse command-line arguments
case "$1" in
    build)
        build
        ;;
    clean)
        clean
        ;;
    rebuild)
        clean
        build
        ;;
    run)
        if [ -z "$2" ]; then
            echo "Usage: $0 run <module>"
            list_modules
            exit 1
        fi
        run_module "$2"
        ;;
    list)
        list_modules
        ;;
    *)
        echo "Usage: $0 {build|clean|rebuild|run <module>|list}"
        exit 1
        ;;
esac
