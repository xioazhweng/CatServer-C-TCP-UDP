cmake_minimum_required(VERSION 3.10)
project(CatServer)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(libs/net/udptransceiver)
add_subdirectory(libs/net/tcptransceiver)

add_subdirectory(src)

add_executable(catserver src/main.cpp)
target_link_libraries(catserver controller)
target_include_directories(catserver PRIVATE src)
