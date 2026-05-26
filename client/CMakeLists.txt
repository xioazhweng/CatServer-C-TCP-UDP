cmake_minimum_required(VERSION 3.10)
project(CatServer)
set(CMAKE_CXX_STANDARD 20)


add_executable( catserver
    src/main.cpp
)
target_link_libraries(catserver udptransceiver)
add_subdirectory(src)