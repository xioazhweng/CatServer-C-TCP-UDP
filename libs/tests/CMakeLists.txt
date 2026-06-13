cmake_minimum_required(VERSION 3.10)
project(tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(udptest
    udp_test.cpp
)

add_executable(tcptest
    tcp_test.cpp
)

target_link_libraries(udptest PRIVATE
    udptransceiver
)
target_link_libraries(tcptest PRIVATE
    tcplib tcpserver tcpstream tcpacceptor
)
