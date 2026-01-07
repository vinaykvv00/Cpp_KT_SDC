cmake_minimum_required(VERSION 3.10)
project(DynamicExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use external_libs/StringLib directly
set(STRINGLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external_libs/StringLib")

add_executable(dynamic_example main.cpp)

# Link to shared library from build folder
target_include_directories(dynamic_example PRIVATE ${STRINGLIB_DIR}/include)
target_link_libraries(dynamic_example PRIVATE ${STRINGLIB_DIR}/build/libextended_string_sdk.so)

# Set RPATH so executable finds the .so at runtime
set_target_properties(dynamic_example PROPERTIES
    BUILD_RPATH "${STRINGLIB_DIR}/build"
    INSTALL_RPATH "${STRINGLIB_DIR}/build"
    BUILD_WITH_INSTALL_RPATH FALSE
)
