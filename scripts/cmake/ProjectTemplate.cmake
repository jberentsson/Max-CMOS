# TODO: Find a way to fix thelibrary imports.
macro(project_template)
    cmake_minimum_required(VERSION "3.19")

    project_name()

    include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

    #############################################################
    # MAX EXTERNAL
    #############################################################

    # Extract package name from directory path.
    #string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
    #project(${THIS_PACKAGE_NAME})

    include_directories( 
        ${C74_INCLUDES}
        ${PROJECT_ROOT}/source/shared
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
    # UNIT TEST
    #############################################################

    include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)

    #############################################################
    # Import Shared Code And Link It To Our Binaries
    #############################################################

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        message("THE PROJECT LIBRARIES ARE: ${PROJECT_LIBRARIES}")
            target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
            target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_LIBRARIES})
            target_link_directories(${PROJECT_NAME}_test PRIVATE ${PROJECT_LIBRARIES})
    endif()
endmacro()
