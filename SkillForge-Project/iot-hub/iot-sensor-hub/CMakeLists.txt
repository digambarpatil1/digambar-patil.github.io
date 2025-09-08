cmake_minimum_required(VERSION 3.16)
project(IoT_Sensor_Hub LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Always enable debug symbols (you can also control via CMAKE_BUILD_TYPE)
#set(CMAKE_BUILD_TYPE Debug)
# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/sensors/dht22sensor.cpp
    src/sensors/PIRsensor.cpp
    src/core/messagebus.cpp
    src/logger/Logger.cpp
    src/plugins/LoggingPlugin.cpp
    src/plugins/filesinkplugin.cpp
    src/adapters/HTTPAdapter.cpp
    src/adapters/MQTTAdapter.cpp
    # LIN related
    src/lin/LinBusSim.cpp
    src/lin/LinNormalizer.cpp
    src/sensors/LinGatewaySensor.cpp        

    src/main.cpp
)


# Executable
add_executable(iot_sensor_hub ${SOURCES})

# Find libraries
find_package(fmt REQUIRED) 

# Link libraries
target_link_libraries(iot_sensor_hub PRIVATE fmt::fmt)
# Link Open-LIN static library
#target_link_libraries(your_target_name PRIVATE ${CMAKE_SOURCE_DIR}/include/lin/lib/libopenlin.a)
# Optional: Enable warnings
if (MSVC)
    target_compile_options(iot_sensor_hub PRIVATE /W4 /permissive-)
else()
    target_compile_options(iot_sensor_hub PRIVATE -Wall -Wextra -Wpedantic -pthread)
endif()

# Copy config folder to build folder after build
add_custom_command(TARGET iot_sensor_hub POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:iot_sensor_hub>/config
)