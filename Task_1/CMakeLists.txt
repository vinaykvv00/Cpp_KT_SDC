cmake_minimum_required(VERSION 3.10)
project(pubsub_simple VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(pubsub
    src/Message.cpp
    src/Topic.cpp
    src/Producer.cpp
    src/Consumer.cpp
    src/Utils.cpp
)

add_executable(pubsub_example main.cpp)
target_link_libraries(pubsub_example PRIVATE pubsub pthread)
