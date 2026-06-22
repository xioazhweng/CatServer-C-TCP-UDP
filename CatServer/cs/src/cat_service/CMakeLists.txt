add_library(catservice INTERFACE CatService.h)
target_include_directories(catservice INTERFACE $(CMAKE_CURRENT_SOURCE_DIR))