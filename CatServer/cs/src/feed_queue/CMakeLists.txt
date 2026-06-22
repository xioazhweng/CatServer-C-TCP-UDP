add_library(feedqueue FeedQueue.h FeedQueue.cpp)
target_link_libraries(feedqueue food)
target_include_directories(feedqueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
