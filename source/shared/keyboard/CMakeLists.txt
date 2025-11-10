cmake_minimum_required(VERSION "3.16")

include(${PROJECT_ROOT}/cmake/ProjectName.cmake)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)

#############################################################
# CATCH2 SETUP
#############################################################

include(${PROJECT_ROOT}/cmake/Catch.cmake)

#############################################################
# KEYBOARD TEST TARGET - FIXED PATHS
#############################################################

message(STATUS "Using Note.cpp at: ${NOTE_CPP_PATH}")

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.cpp
)

add_executable(${PROJECT_NAME}
    ${TEST_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
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

message(STATUS "${PROJECT_NAME} test target created and registered with CTest")
