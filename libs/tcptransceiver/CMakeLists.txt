
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_subdirectory(include)
add_subdirectory(libs)

add_library(tcplib INTERFACE)
target_include_directories(tcplib INTERFACE include)
target_link_libraries(tcplib INTERFACE tcpserver tcpclient tcpstream)