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
    # Add the parent directory of thulr/source to include path
    # This makes #include "BinaryCounter/BinaryCounter.hpp" work
    #############################################################

    # Get the parent directory of thulr/source (should be thulr/)
    get_filename_component(THULR_PARENT_PATH ${THULR_PATH} DIRECTORY)

    # Apply include directories to all targets
    foreach(TARGET ${PROJECT_LIBRARIES_TARGETS})
        target_include_directories(${TARGET} PRIVATE
            ${THULR_PARENT_PATH}  # This makes folder/file.hpp includes work
            ${THULR_PATH}         # Keep this for direct includes
        )
    endforeach()

    #############################################################
    # Link the libraries to the targets.
    #############################################################

    foreach(PLT ${PROJECT_LIBRARIES_TARGETS})
        foreach(LIB ${PROJECT_LIBRARIES})
            if(TARGET ${LIB})
                target_link_libraries(${PLT} PRIVATE ${LIB})
            endif()
        endforeach()
    endforeach()

    #############################################################
    # Done!
    #############################################################
endmacro()
