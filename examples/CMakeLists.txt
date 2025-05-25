cmake_minimum_required(VERSION 3.20)

option(BUILD_EXAMPLES "Build example programs" ON)

# Build examples if enabled
if(BUILD_EXAMPLES)
    set(SOURCES
        main.cpp
    )
    add_executable(example ${SOURCES})
    target_link_libraries(example PRIVATE Multithreading)
endif()
