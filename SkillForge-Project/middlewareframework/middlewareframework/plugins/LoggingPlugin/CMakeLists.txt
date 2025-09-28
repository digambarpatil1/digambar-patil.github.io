
find_package(fmt REQUIRED)

add_library(LoggingPlugin SHARED
      LoggingPlugin.cpp
)

target_include_directories(LoggingPlugin PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LoggingPlugin PRIVATE
    middlewarecore 
     fmt::fmt
)