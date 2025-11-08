cmake_minimum_required(VERSION 3.19)

# Now include min-pretarget.cmake
set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# Now include min-pretarget.cmake which might use MAXAPI_LIB
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

#############################################################
# MAX EXTERNAL
#############################################################

include_directories( 
    ${C74_INCLUDES}
)

# SOURCE FILES FOR MAX EXTERNAL ONLY
set( SOURCE_FILES
    ${PROJECT_NAME}.hpp
    ${PROJECT_NAME}.cpp
)

add_library(
    ${PROJECT_NAME} 
    MODULE
    ${SOURCE_FILES}
)

# Make sure to link the MaxAPI library
target_link_libraries(${PROJECT_NAME} PRIVATE ${MAXAPI_LIB})

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

#############################################################
# UNIT TEST (separate executable - NO shared sources)
#############################################################

# Test executable only compiles the test file
add_executable(${PROJECT_NAME}_test tests/${PROJECT_NAME}_test.cpp)

# Link test executable with the same MaxAPI library
target_link_libraries(${PROJECT_NAME}_test PRIVATE 
    Catch2::Catch2WithMain
    ${MAXAPI_LIB}
)

# Include test configuration
include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
