cmake_minimum_required(VERSION "3.19")

project_name()

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

#############################################################
# MAX EXTERNAL
#############################################################

include_directories( 
    ${C74_INCLUDES}
    ${PROJECT_ROOT}/source/shared
)

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
# UNIT TEST
#############################################################

include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)

#############################################################
# Import Shared Code And Link It To Our Binaries
#############################################################

set(PROJECT_LIBRARIES BinaryCounter Counter)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
target_link_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_ROOT}/source/shared/Counter
    ${PROJECT_ROOT}/source/shared/BinaryCounter
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_LIBRARIES})
target_link_directories(${PROJECT_NAME}_test PRIVATE
    ${PROJECT_ROOT}/source/shared/Counter
    ${PROJECT_ROOT}/source/shared/BinaryCounter
)
