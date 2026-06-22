add_library(petqueue PetQueue.h PetQueue.cpp)
target_include_directories(petqueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
