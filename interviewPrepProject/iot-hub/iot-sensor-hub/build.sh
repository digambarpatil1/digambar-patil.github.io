#!/bin/bash
set -e  # stop on error

BUILD_DIR="build"

# Create build directory if not exists
if [ ! -d "$BUILD_DIR" ]; then
  mkdir $BUILD_DIR
fi

cd $BUILD_DIR

# Configure + Build
cmake ..  -DCMAKE_BUILD_TYPE=debug
make -j$(nproc)

echo "✅ Build complete!"
echo "👉 Executables are inside the 'build' directory."

# Run specific target if provided
if [ $# -gt 0 ]; then
  TARGET=$1
  if [ -f "./$TARGET" ]; then
    echo "🚀 Running $TARGET ..."
    ./$TARGET
  else
    echo "⚠️ Executable '$TARGET' not found. Available executables:"
    ls -1 | grep -v CMake
  fi
fi

