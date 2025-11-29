macro(project_template)
    #############################################################
    # Max Module Template
    #############################################################

    project_name()

    include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

    set(THULR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../thulr/source)
    
    # Debug info for GitHub Actions
    message(STATUS "=== Configuring ${PROJECT_NAME} ===")

    #############################################################
    # MAX EXTERNAL
    #############################################################

    include_directories( 
        ${THULR_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set( SOURCE_FILES
        ${PROJECT_NAME}.hpp
    )

    add_library(
        ${PROJECT_NAME}
        MODULE
        ${SOURCE_FILES}
    )

    add_library(
        ${PROJECT_NAME}_static
        STATIC
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
        ${PROJECT_NAME}_static
    )

    #############################################################
    # Comprehensive include path setup
    #############################################################
    
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${C74_INCLUDES})

    # Get the parent directory of thulr/source
    get_filename_component(THULR_PARENT_PATH ${THULR_PATH} DIRECTORY)
    
    # List all possible include paths
    set(ALL_INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${THULR_PARENT_PATH}
        ${THULR_PATH}
        ${THULR_PATH}/Utils
    )

    # Apply to all targets
    foreach(TARGET ${PROJECT_LIBRARIES_TARGETS})
        target_include_directories(${TARGET} PRIVATE ${ALL_INCLUDE_PATHS})
    endforeach()

    #############################################################
    # Link the libraries to the targets
    #############################################################

    foreach(PLT ${PROJECT_LIBRARIES_TARGETS})
        foreach(LIB ${PROJECT_LIBRARIES})
            if(TARGET ${LIB}_static)
                target_link_libraries(${PLT} PRIVATE ${LIB}_static)
                message(STATUS "Linked ${PLT} with ${LIB}_static")
            elseif(TARGET ${LIB})
                target_link_libraries(${PLT} PRIVATE ${LIB})
                message(STATUS "Linked ${PLT} with ${LIB}")
            else()
                message(WARNING "Library target ${LIB} not found for linking with ${PLT}")
            endif()
        endforeach()
    endforeach()

    #############################################################
    # FIX TEST EXECUTABLE LINKING
    #############################################################

    if(TARGET ${PROJECT_NAME}_test)
        # Link the test executable with the static library
        target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_static)
        
        # Also link with all base libraries
        foreach(LIB ${PROJECT_LIBRARIES})
            if(TARGET ${LIB}_static)
                target_link_libraries(${PROJECT_NAME}_test PRIVATE ${LIB}_static)
                message(STATUS "Linked ${PROJECT_NAME}_test with ${LIB}_static")
            elseif(TARGET ${LIB})
                target_link_libraries(${PROJECT_NAME}_test PRIVATE ${LIB})
                message(STATUS "Linked ${PROJECT_NAME}_test with ${LIB}")
            endif()
        endforeach()
        
        # Add include directories for test executable
        target_include_directories(${PROJECT_NAME}_test PRIVATE ${ALL_INCLUDE_PATHS})
    endif()

    #############################################################
    # Fix compiler warnings - DON'T treat warnings as errors
    #############################################################
    
    # Add compiler flags but DON'T use -Werror
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Enable warnings but don't treat them as errors
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
        target_compile_options(${PROJECT_NAME}_static PRIVATE -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)

        if(TARGET ${PROJECT_NAME}_test)
            target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
        endif()
    endif()

    #############################################################
    # Done!
    #############################################################
endmacro()
