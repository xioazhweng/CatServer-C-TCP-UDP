add_library(tcpstream TCPStreamImpl.h TCPStreamImpl.cpp)
target_link_libraries(tcpstream tcpstream_i tcpclient)
target_include_directories(tcpstream PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))