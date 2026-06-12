add_library(controller Controller.cpp Controller.h)

target_link_libraries(controller
    PUBLIC
        udptransceiver
        tcptranceiver
)