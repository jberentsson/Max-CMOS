macro(library_template PROJECT_LIBRARIES)
    cmake_minimum_required(VERSION "3.19")

    project_name()

    set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)

    #############################################################
    # LIBRARY TARGET
    #############################################################

    add_library(${PROJECT_NAME} STATIC 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}.hpp
    )

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PT ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME} ${PL})
        endforeach()
    endif()

    #############################################################
    # TEST TARGET
    #############################################################

    set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp
    )

    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    # Include directories
    #target_include_directories(${PROJECT_NAME}_test PRIVATE
    #    ${CMAKE_CURRENT_SOURCE_DIR}
    #    ${CMAKE_CURRENT_SOURCE_DIR}../
    #)

    # Set output directories for ALL configurations
    set_target_properties(${PROJECT_NAME}_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/tests
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    #############################################################
    # AUTOMATIC TEST REGISTRATION WITH CTEST
    #############################################################

    include(${PROJECT_ROOT}/cmake/AddTests.cmake)
endmacro()
