macro(project_template)
    #############################################################
    # Max Module Template
    #############################################################

    project_name()

    include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

    set(THULR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../thulr/source)
    
    # Debug info for GitHub Actions
    message(STATUS "=== Configuring ${PROJECT_NAME} ===")
    message(STATUS "THULR_PATH: ${THULR_PATH}")
    message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

    #############################################################
    # MAX EXTERNAL
    #############################################################

    include_directories( 
        #${C74_INCLUDES}
        ${THULR_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}
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
    # Comprehensive include path setup for GitHub Actions
    #############################################################
    
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
        ${C74_INCLUDES}
    )

    # Get the parent directory of thulr/source
    get_filename_component(THULR_PARENT_PATH ${THULR_PATH} DIRECTORY)
    
    # List all possible include paths
    set(ALL_INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${THULR_PARENT_PATH}
        ${THULR_PATH}
        ${THULR_PATH}/Utils
        ${CMAKE_CURRENT_SOURCE_DIR}/../../thulr
        ${CMAKE_CURRENT_SOURCE_DIR}/../thulr  
        ${CMAKE_CURRENT_SOURCE_DIR}/thulr
        ${CMAKE_CURRENT_SOURCE_DIR}../
    )

    # Debug: Check if Utils directory exists
    if(EXISTS ${THULR_PATH}/Utils)
        message(STATUS "Utils directory found: ${THULR_PATH}/Utils")
        file(GLOB UTILS_FILES ${THULR_PATH}/Utils/*.hpp)
        message(STATUS "Utils header files:")
        foreach(UTIL_FILE ${UTILS_FILES})
            message(STATUS "  - ${UTIL_FILE}")
        endforeach()
    else()
        message(WARNING "Utils directory NOT found: ${THULR_PATH}/Utils")
    endif()

    # Apply to all targets
    foreach(TARGET ${PROJECT_LIBRARIES_TARGETS})
        target_include_directories(${TARGET} PRIVATE ${ALL_INCLUDE_PATHS})
        message(STATUS "Include paths for ${TARGET}:")
        foreach(INCLUDE_PATH ${ALL_INCLUDE_PATHS})
            message(STATUS "  - ${INCLUDE_PATH}")
        endforeach()
    endforeach()

    #############################################################
    # Link the libraries to the targets.
    #############################################################

    foreach(PLT ${PROJECT_LIBRARIES_TARGETS})
        foreach(LIB ${PROJECT_LIBRARIES})
            if(TARGET ${LIB})
                target_link_libraries(${PLT} PRIVATE ${LIB})
                message(STATUS "Linked ${PLT} with ${LIB}")
            endif()
        endforeach()
    endforeach()

    #############################################################
    # Done!
    #############################################################
endmacro()
