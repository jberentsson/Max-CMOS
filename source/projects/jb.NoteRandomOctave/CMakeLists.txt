cmake_minimum_required(VERSION "3.19")

project_name()

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

#############################################################
# MAX EXTERNAL
#############################################################

include_directories( 
    ${C74_INCLUDES}
    ${PROJECT_ROOT}/source/shared
)

set(SOURCE_FILES
    ${PROJECT_NAME}.cpp
    ${PROJECT_NAME}.hpp
)

add_library(${PROJECT_NAME} MODULE 
    ${SOURCE_FILES}
    ${include_directories}
)

#############################################################
# UNIT TEST
#############################################################

target_link_libraries(${PROJECT_NAME} PRIVATE Keyboard)

#############################################################
# Import Shared Code
#############################################################

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)
include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)

target_link_libraries(${PROJECT_NAME}_test PRIVATE Keyboard)
target_include_directories(${PROJECT_NAME}_test PRIVATE 
    ${PROJECT_ROOT}/source/shared/Keyboard
)
