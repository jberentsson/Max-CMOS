cmake_minimum_required(VERSION 3.19)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# Update the variable.
set_max_libs()

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

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

#############################################################
# UNIT TEST (separate executable - NO shared sources)
#############################################################

# Test executable only compiles the test file
add_executable(${PROJECT_NAME}_test tests/${PROJECT_NAME}_test.cpp)

# Try to get the link libraries from the main target and use them for the test
get_target_property(MAX_LIBS ${PROJECT_NAME} LINK_LIBRARIES)

target_link_libraries(${PROJECT_NAME}_test PRIVATE 
    Catch2::Catch2WithMain
    ${MAX_LIBS}
)

# Include test configuration
include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
