cmake_minimum_required(VERSION 3.10)

project(LSP_Correct)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(lsp_app main.cpp ${SRC_FILES})
