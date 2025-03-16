enable_testing()

add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE my_lib)
add_test(NAME MyTest COMMAND test_main)
