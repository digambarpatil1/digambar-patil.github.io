cmake_minimum_required(VERSION 3.16)
project(VehicleMiddleware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# necessary if static libs will be linked into shared libs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  

# Enable gprof
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -Wall -Wextra -Wpedantic")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")

# Add include directory for all targets
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(fmt REQUIRED)

# -------------------------------
# Core Library
# -------------------------------
add_library(middlewarecore SHARED
    src/lin/LinBusSim.cpp
    src/pluginManager/PluginManager.cpp
    src/sensors/LinGatewaySensor.cpp
    src/lin/LinNormalizer.cpp
    src/core/messagebus.cpp
    src/core/Logger.cpp      
)

target_include_directories(middlewarecore PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# -------------------------------
# Main Executable
# -------------------------------
add_executable(main
    src/main.cpp 
)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/plugins   
)

target_link_libraries(main PRIVATE middlewarecore pthread fmt::fmt)

# -------------------------------
# Plugins
# -------------------------------
add_subdirectory(plugins/LinGatewayPlugin)
add_subdirectory(plugins/LoggingPlugin)
add_subdirectory(plugins/MqttAdapterPlugin)

# -------------------------------
# Google Test Integration
# -------------------------------
include(FetchContent)
enable_testing()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

add_executable(AllTests ${TEST_SOURCES})

target_link_libraries(AllTests
    PRIVATE
        middlewarecore
        gtest_main
        pthread
        fmt::fmt
)

target_include_directories(AllTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

gtest_discover_tests(AllTests)
