cmake_minimum_required(VERSION 3.10)
project(SharedMemoryReader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(reader main.cpp)

# Link with rt library for POSIX shared memory functions
target_link_libraries(reader rt)