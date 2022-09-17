cmake_minimum_required(VERSION 3.23)
project(lib)

set(CMAKE_CXX_STANDARD 14)


add_library(lib SHARED src/lockSafe_queue.cpp)
target_include_directories(lib PUBLIC lockDS)

add_executable(test src/test.cpp)
target_link_libraries(test lib)