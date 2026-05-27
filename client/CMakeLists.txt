cmake_minimum_required(VERSION 3.10)
project(CatClient)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

add_executable(catclient
    src/main.cpp
)
add_subdirectory(src)
target_link_libraries(catclient PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    controller
)