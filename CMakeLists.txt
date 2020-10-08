# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project(encoder)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++17")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)

# Include sub-dir.
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(dependencyLinux)



