cmake_minimum_required(VERSION 3.10)
project(CatClient)


set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

add_executable(catclient
    src/main.cpp
)
add_subdirectory(src)
add_subdirectory(tests)
target_link_libraries(catclient PRIVATE
    clientui
    controller
     nlohmann_json::nlohmann_json
)
