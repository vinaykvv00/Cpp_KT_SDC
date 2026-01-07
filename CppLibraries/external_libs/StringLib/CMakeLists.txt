cmake_minimum_required(VERSION 3.10)
project(ExtendedStringSDK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCES src/extended_string_sdk.cpp)
set(LIB_HEADERS include/extended_string_sdk.h)

add_library(extended_string_sdk_static STATIC ${LIB_SOURCES})
target_include_directories(extended_string_sdk_static
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(extended_string_sdk_static PROPERTIES
    OUTPUT_NAME extended_string_sdk
    VERSION ${PROJECT_VERSION}
)

add_library(extended_string_sdk_shared SHARED ${LIB_SOURCES})
target_include_directories(extended_string_sdk_shared
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(extended_string_sdk_shared PROPERTIES
    OUTPUT_NAME extended_string_sdk
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
