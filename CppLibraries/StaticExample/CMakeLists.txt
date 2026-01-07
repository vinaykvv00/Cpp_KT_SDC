cmake_minimum_required(VERSION 3.10)
project(StaticExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(STRINGLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external_libs/StringLib")

add_executable(static_example main.cpp)

target_include_directories(static_example PRIVATE ${STRINGLIB_DIR}/include)
target_link_libraries(static_example PRIVATE ${STRINGLIB_DIR}/build/libextended_string_sdk.a)
