cmake_minimum_required(VERSION 3.20)
project(Multithreading VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to build as a compiled library (static or shared) or header-only
option(BUILD_SHARED_LIBS "Build Multithreading as a shared library" OFF)

set(SOURCES
    include/multithreading/threadpool.h
    src/threadpool.cpp
)

add_library(Multithreading ${SOURCES})
target_include_directories(Multithreading
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>

    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/multithreading>
)

add_subdirectory(examples)

# Installation rules
include(GNUInstallDirs)
install(TARGETS Multithreading
    EXPORT MultithreadingTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/Multithreading
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export CMake configuration
install(EXPORT MultithreadingTargets
    FILE MultithreadingTargets.cmake
    NAMESPACE Multithreading::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Multithreading
)

# Generate and install package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MultithreadingConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/MultithreadingConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MultithreadingConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Multithreading
)
