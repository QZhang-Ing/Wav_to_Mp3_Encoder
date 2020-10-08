# CMakeList.txt : CMake project for mp3_encoder, include source files and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add main.cpp to this project's executable.
add_executable(${PROJECT_NAME} main.cpp)

add_library(source mp3_encoder.cpp thread_helper.cpp)
target_include_directories(source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} source header libmp3lame.a Threads::Threads)
