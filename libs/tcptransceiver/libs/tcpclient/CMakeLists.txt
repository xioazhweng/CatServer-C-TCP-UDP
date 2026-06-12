add_library(tcpclient TCPClient.h TCPClient.cpp)
target_link_libraries(tcpclient tcpconnector tcpstream_i)
