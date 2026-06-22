add_library(srvresolver SRVResolver.h SRVResolver.cpp)
target_include_directories(srvresolver PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))