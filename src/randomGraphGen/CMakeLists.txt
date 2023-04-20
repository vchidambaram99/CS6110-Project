add_executable(RandomGraphGen RandomGraphGenMain.cpp)
target_include_directories(RandomGraphGen PUBLIC ${INCLUDE_DIR})
target_link_libraries(RandomGraphGen Common)
