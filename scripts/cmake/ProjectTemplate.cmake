macro(project_template)
    #############################################################
    # Max Module Template
    #############################################################

    project_name()

    include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

    set(THULR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../thulr/source)

    #############################################################
    # MAX EXTERNAL
    #############################################################

    include_directories( 
        ${C74_INCLUDES}
        ${THULR_PATH}
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

    set(PROJECT_LIBRARIES_TARGETS
        ${PROJECT_NAME}
        ${PROJECT_NAME}_test
    )

    #############################################################
    # Link the libraries to the targets.
    #############################################################

    foreach(PLT ${PROJECT_LIBRARIES_TARGETS})
        foreach(LIB ${PROJECT_LIBRARIES})
            target_link_libraries(${PLT} PRIVATE ${LIB})
            target_link_directories(${PLT} PRIVATE ${THULR_PATH}/${LIB})
        endforeach()
    endforeach()

    #############################################################
    # Done!
    #############################################################
endmacro()
