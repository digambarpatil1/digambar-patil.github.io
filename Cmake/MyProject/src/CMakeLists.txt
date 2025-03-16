add_library(my_lib STATIC lib.cpp)
target_include_directories(my_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(my_lib PUBLIC fmt)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE my_lib)
