add_library(controller Controller.cpp Controller.h)
target_link_libraries(controller PUBLIC udptransceiver udptransceiver_i tcpclient tcpstream)
target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
