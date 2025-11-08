# ========================================
# CATCH2 V3 - MULTI-HEADER LIBRARY SETUP
# ========================================

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

# Add Catch2 as a subdirectory to build it as a library
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/externals/Catch2")

# Use Catch2's provided CMake targets
macro(add_catch_test TEST_TARGET TEST_SOURCE)
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    
    # Link against Catch2's main target - this handles ALL includes and dependencies
    target_link_libraries(${TEST_TARGET} PRIVATE 
        Catch2::Catch2WithMain  # This provides main() and all includes
    )
    
    # Only link your library if it exists
    if(TARGET ${THIS_FOLDER_NAME})
        target_link_libraries(${TEST_TARGET} PRIVATE ${THIS_FOLDER_NAME})
    endif()
    
    enable_testing()
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endmacro()

message(STATUS "Catch2 multi-header library configured")