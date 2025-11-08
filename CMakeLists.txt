cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)

if(NOT DEFINED ENV{${MAX_SDK_PATH}})
    set(MAX_SDK_PATH $ENV{MAX_SDK_PATH})
endif()

# Now MAX_LIBS should be available
message(STATUS "Max libraries: ${MaxAPI_LIB}")

# Extract package name from directory path.
string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME})

# Enable testing framework.
enable_testing()

# Include external libraries.
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CATCH_PATH ${PROJECT_ROOT}/source/catch2/include)
set(MAX_LIBS "")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SetMaxLibs.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Catch2Install.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Main.cmake)

# Comment the line below if you want automatic cmake regeneration enabled.
set(CMAKE_SUPPRESS_REGENERATION true)
