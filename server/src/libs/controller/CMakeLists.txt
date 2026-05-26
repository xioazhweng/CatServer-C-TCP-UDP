add_library(controller Controller.h Controller.cpp)
target_link_libraries(controller udptransceiver feedqueue logger)