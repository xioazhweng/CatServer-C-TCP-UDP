add_library(tcpstream TCPStreamImpl.h TCPStreamImpl.cpp)
target_link_libraries(tcpstream tcpstream_i)
