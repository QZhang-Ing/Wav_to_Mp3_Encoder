# CMakeList.txt : CMakeList for all header files 
# in /include directory.
#
cmake_minimum_required(VERSION 3.8)

add_library(header INTERFACE)
target_include_directories(header INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


