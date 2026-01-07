cmake_minimum_required(VERSION 3.10)
project(DynamicRuntime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dynamic_runtime main.cpp)

target_link_libraries(dynamic_runtime PRIVATE dl)

