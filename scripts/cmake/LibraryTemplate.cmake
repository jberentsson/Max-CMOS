macro(library_template PROJECT_LIBRARIES)
    cmake_minimum_required(VERSION "3.19")

    if(NOT DEFINED PROJECT_NAME)
        message(FATAL_ERROR "PROJECT_NAME must be defined before calling library_template")
    endif()

    #############################################################
    # LIBRARY TARGET
    #############################################################

    add_library(${PROJECT_NAME} STATIC 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}.hpp
    )

    # Force static runtime for MSVC
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link with Counter for dependent libraries
    if(NOT ${PROJECT_NAME} STREQUAL "Counter")
        target_link_libraries(${PROJECT_NAME} Counter)
    endif()

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PL ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME} ${PL})
        endforeach()
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    #############################################################
    # TEST TARGET - NO CATCH2 LIBRARY LINKING
    #############################################################

    set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp
    )

    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    # TODO: Simplify this.
    if(TARGET Catch2::Catch2WithMain)
        target_link_libraries(${PROJECT_NAME}_test PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})
    elseif(TARGET Catch2::Catch2)
        target_link_libraries(${PROJECT_NAME}_test PRIVATE Catch2::Catch2 ${PROJECT_NAME})
    elseif(TARGET Catch2)
        target_link_libraries(${PROJECT_NAME}_test PRIVATE Catch2 ${PROJECT_NAME})
    else()
        message(FATAL_ERROR "No Catch2 target found!")
    endif()

    target_include_directories(${PROJECT_NAME}_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/../extern
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/Counter
    )

    # Force static runtime for test executable
    if(MSVC)
        set_target_properties(${PROJECT_NAME}_test PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    set_target_properties(${PROJECT_NAME}_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
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

    if(EXISTS ${PROJECT_ROOT}/scripts/cmake/AddTests.cmake)
        include(${PROJECT_ROOT}/scripts/cmake/AddTests.cmake)
    endif()

endmacro()
