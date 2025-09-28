add_library(LinGatewayPlugin SHARED
    LinGatewayPlugin.cpp
)

target_include_directories(LinGatewayPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(LinGatewayPlugin PRIVATE
    middlewarecore
    pthread
)