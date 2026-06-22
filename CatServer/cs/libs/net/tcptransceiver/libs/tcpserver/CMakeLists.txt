add_library(tcpserver TCPServer.h TCPServer.cpp)
target_link_libraries(tcpserver tcpacceptor tcpstream tcpstream_i)
target_include_directories(tcpserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
