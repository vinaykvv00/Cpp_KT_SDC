cmake_minimum_required(VERSION 3.10)
project(ModbusProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

add_executable(ModbusProject
    main.cpp
    src/ModbusServer.cpp
    src/ModbusClient.cpp
)

target_link_libraries(ModbusProject ${LIBMODBUS_LIBRARIES})
