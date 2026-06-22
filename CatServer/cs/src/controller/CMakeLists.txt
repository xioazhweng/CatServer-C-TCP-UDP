add_library(controller Controller.h Controller.cpp)
target_link_libraries(controller udptransceiver tcpserver feedqueue petqueue logger catservice)
target_include_directories(controller PUBLIC ${CMAKE_SOURCE_DIR}/src)
