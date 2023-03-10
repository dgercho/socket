project(socket_tests)

add_executable(
    socket_tests 
    socket_tests.cpp
)

target_link_libraries(
    socket_tests
    Socket
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(socket_tests)
