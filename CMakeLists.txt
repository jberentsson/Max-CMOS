cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Extract package name from directory path.
string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME})

# Enable testing framework.
enable_testing()

# Include external libraries.
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CATCH_PATH ${PROJECT_ROOT}/source/catch2/include)

# ---------------------------------------
# Shared libraries
# ---------------------------------------

file(GLOB_RECURSE SHARED_SOURCES 
    ${PROJECT_ROOT}/source/shared/*/*.cpp
    ${PROJECT_ROOT}/source/shared/*/*.hpp
)

# Ignore Test Files
list(FILTER SHARED_SOURCES EXCLUDE REGEX ".*_test\\.cpp$")
set(SHARED_LIB_PATH ${PROJECT_ROOT}/source/shared)

#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Catch2Install.cmake)
#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Catch2Add.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Main.cmake)

# Comment the line below if you want automatic cmake regeneration enabled.
set(CMAKE_SUPPRESS_REGENERATION true)
