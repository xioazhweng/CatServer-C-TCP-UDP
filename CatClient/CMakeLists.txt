cmake_minimum_required(VERSION 3.10)
project(CatClient)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/net)
add_subdirectory(src)

add_executable(catclient src/main.cpp)
target_link_libraries(catclient PRIVATE clientui controller)
target_include_directories(catclient PRIVATE src)
