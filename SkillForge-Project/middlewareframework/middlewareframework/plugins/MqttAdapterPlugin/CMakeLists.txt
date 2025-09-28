

# Find libraries
# List of required packages
set(REQUIRED_PACKAGES
    OpenSSL
    PahoMqttCpp 
)
# Find all required packages
foreach(pkg IN LISTS REQUIRED_PACKAGES)
    find_package(${pkg} REQUIRED)
endforeach()

add_library(MqttAdapterPlugin SHARED
MQTTAdapter.cpp
)

target_include_directories(MqttAdapterPlugin PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MqttAdapterPlugin PRIVATE
    middlewarecore
    PahoMqttCpp::paho-mqttpp3
)