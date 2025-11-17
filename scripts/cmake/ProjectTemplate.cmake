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
    # Fix include directories for ALL targets
    #############################################################

    # Get all the library subdirectories in thulr
    file(GLOB THULR_LIBRARIES RELATIVE ${THULR_PATH} ${THULR_PATH}/*)
    set(THULR_INCLUDE_DIRS "")
    
    foreach(LIB ${THULR_LIBRARIES})
        if(IS_DIRECTORY ${THULR_PATH}/${LIB})
            list(APPEND THULR_INCLUDE_DIRS ${THULR_PATH}/${LIB})
        endif()
    endforeach()

    # Add Utils directory if it exists
    if(EXISTS ${THULR_PATH}/Utils)
        list(APPEND THULR_INCLUDE_DIRS ${THULR_PATH}/Utils)
    endif()

    # Apply include directories to all targets
    foreach(TARGET ${PROJECT_LIBRARIES_TARGETS})
        target_include_directories(${TARGET} PRIVATE
            ${THULR_PATH}
            ${THULR_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../thulr/source
            ${CMAKE_CURRENT_SOURCE_DIR}/../thulr/source
            ${CMAKE_CURRENT_SOURCE_DIR}/source/thulr/source
        )
    endforeach()

    #############################################################
    # Link the libraries to the targets.
    #############################################################

    foreach(PLT ${PROJECT_LIBRARIES_TARGETS})
        foreach(LIB ${PROJECT_LIBRARIES})
            # Find the actual library target
            if(TARGET ${LIB})
                target_link_libraries(${PLT} PRIVATE ${LIB})
            endif()
        endforeach()
    endforeach()

    #############################################################
    # Done!
    #############################################################
endmacro()
