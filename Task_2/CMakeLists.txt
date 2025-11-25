cmake_minimum_required(VERSION 3.10)
project(UDPChatRoom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIB_SOURCES
    src/Message.cpp
    src/Room.cpp
    src/Utils.cpp
    src/ChatServer.cpp
    src/ChatClient.cpp
)

add_library(chatlib STATIC ${LIB_SOURCES})

add_executable(chat_server server_main.cpp)
target_link_libraries(chat_server chatlib pthread)

add_executable(chat_user user_main.cpp)
target_link_libraries(chat_user chatlib pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
