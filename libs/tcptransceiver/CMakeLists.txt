
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
include_directories(include)
add_subdirectory(include)
add_subdirectory(libs)

add_library(tcptranceiver INTERFACE)

target_include_directories(tcptranceiver
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tcptranceiver
    INTERFACE
        tcpclient
        tcpstream
        tcpserver
)