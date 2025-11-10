cmake_minimum_required(VERSION "3.16")

string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME})

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)

#############################################################
# CATCH2 SETUP
#############################################################

include(${PROJECT_ROOT}/cmake/Catch.cmake)

#############################################################
# KEYBOARD TEST TARGET - FIXED PATHS
#############################################################

#Get the absolute path to Note.cpp
# set(NOTE_CPP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../note/Note.cpp)
# 
#Verify the file exists
# if(NOT EXISTS ${NOTE_CPP_PATH})
    # message(WARNING "Note.cpp not found at: ${NOTE_CPP_PATH}")
    # Try alternative path
    # set(NOTE_CPP_PATH ${CMAKE_SOURCE_DIR}/source/shared/note/Note.cpp)
    # if(NOT EXISTS ${NOTE_CPP_PATH})
        # message(FATAL_ERROR "Note.cpp not found at: ${NOTE_CPP_PATH}")
    # endif()
# endif()

message(STATUS "Using Note.cpp at: ${NOTE_CPP_PATH}")

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.cpp
    ${NOTE_CPP_PATH}  # Use the verified path
)

add_executable(${PROJECT_NAME}
    ${TEST_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..  # For note/Note.hpp
    ${CATCH2_INCLUDE_DIR}
)

# Set output directories for ALL configurations
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/tests
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

#############################################################
# AUTOMATIC TEST REGISTRATION WITH CTEST
#############################################################

enable_testing()
add_test(
    NAME ${PROJECT_NAME}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)

set_tests_properties(${PROJECT_NAME} 
    PROPERTIES 
        TIMEOUT 30 
        LABELS "unit;shared"
)

message(STATUS "Keyboard test target created and registered with CTest")