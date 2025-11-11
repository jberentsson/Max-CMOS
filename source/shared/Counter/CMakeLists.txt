cmake_minimum_required(VERSION "3.19")

include(${PROJECT_ROOT}/cmake/ProjectName.cmake)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)

#############################################################
# KEYBOARD TEST TARGET - FIXED PATHS
#############################################################


#set(PROJECT_LIBRARIES Catch2::Catch2WithMain)

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp
)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directories for ALL configurations
set_target_properties(${PROJECT_NAME}_test PROPERTIES
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

include(${PROJECT_ROOT}/cmake/AddTests.cmake)


add_library(${PROJECT_NAME}
    ${PROJECT_NAME}.hpp
    ${PROJECT_NAME}.cpp
)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# If you're building a static library on Windows, make sure it produces .lib
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
    )
endif()
