cmake_minimum_required(VERSION 3.14)
project(FutureAsyncPromise VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -pthread)
endif()

# Find Threads package
find_package(Threads REQUIRED)

# List of all executables
set(EXAMPLES
    01_future_basics
    02_future_member_functions
    03_async_complete
    04_promise_complete
    05_combined_advanced
    06_packaged_task_complete
)

# Create executable for each example
foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} PRIVATE Threads::Threads)
    
    # Set output directory
    set_target_properties(${EXAMPLE} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()

# Print build information
message(STATUS "==================================================")
message(STATUS "C++ Future/Async/Promise Examples")
message(STATUS "==================================================")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Examples: ${EXAMPLES}")
message(STATUS "==================================================")

# Add custom target to run all examples
add_custom_target(run_all
    COMMENT "Running all examples..."
    DEPENDS ${EXAMPLES}
)

foreach(EXAMPLE ${EXAMPLES})
    add_custom_command(TARGET run_all POST_BUILD
        COMMAND echo "\\n========== Running ${EXAMPLE} =========="
        COMMAND ${CMAKE_BINARY_DIR}/bin/${EXAMPLE}
        COMMENT "Running ${EXAMPLE}"
    )
endforeach()
