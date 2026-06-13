add_library(tcpclient TCPClient.h TCPClient.cpp)
target_link_libraries(tcpclient tcpconnector tcpstream_i)
target_include_directories(tcpclient PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))