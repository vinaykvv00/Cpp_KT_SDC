cmake_minimum_required(VERSION 3.10)
project(DIPExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(dip_example
    main.cpp
    src/MySQLDatabase.cpp
    src/MongoDBDatabase.cpp
    src/UserService.cpp
)
