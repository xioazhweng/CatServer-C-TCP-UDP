cmake_minimum_required(VERSION 3.10)
project(tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(test
    udp_test.cpp
)

target_link_libraries(test PRIVATE
    udptransceiver
)
