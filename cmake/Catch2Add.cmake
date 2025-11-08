# cmake/Catch2Add.cmake
macro(add_catch_test TEST_TARGET TEST_SOURCE)
    message(STATUS "Adding Catch2 test: ${TEST_TARGET}")
    
    # Create test executable
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    
    # THIS IS THE CRITICAL MISSING LINE:
    target_include_directories(${TEST_TARGET} PRIVATE 
        ${PROJECT_ROOT}/build/_deps/catch2-src
    )
    
    enable_testing()
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
    
    message(STATUS "✓ Successfully added test: ${TEST_TARGET}")
endmacro()
