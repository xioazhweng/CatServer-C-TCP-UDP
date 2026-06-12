add_library(udptransceiver UDPTransceiver.h UDPTransceiver.cpp)
target_link_libraries(udptransceiver udptransceiver_i)
include_directories(../include)