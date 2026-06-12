add_library(tcpserver TCPServer.h TCPServer.cpp)
target_link_libraries(tcpserver tcpacceptor tcpstream_i)
