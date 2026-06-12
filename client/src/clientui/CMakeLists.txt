include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

add_library(clientui ClientUI.h ClientUI.cpp)
target_link_libraries(clientui 
    ftxui::screen
    ftxui::dom
    ftxui::component)
