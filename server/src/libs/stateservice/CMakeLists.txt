include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)
add_library(jsonstateservice JSONStateService.h JSONStateService.cpp)
target_link_libraries(jsonstateservice PRIVATE nlohmann_json::nlohmann_json)