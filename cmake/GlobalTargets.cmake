# Global targets function
function(add_global_targets PROJECT_NAME)
    # Global target for building all externals
    if(NOT TARGET build_all_externals)
        add_custom_target(build_all_externals
            COMMENT "Building all Max externals"
        )
    endif()
    
    # Global target for building all tests  
    if(NOT TARGET build_all_tests)
        add_custom_target(build_all_tests
            COMMENT "Building all tests"
        )
    endif()
    
    # Global target for running all tests
    if(NOT TARGET run_all_tests)
        add_custom_target(run_all_tests
            COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
            COMMENT "Running all tests"
        )
    endif()
    
    # Add this project to the global targets
    if(TARGET build_external_${PROJECT_NAME})
        add_dependencies(build_all_externals build_external_${PROJECT_NAME})
    endif()
    
    if(TARGET build_tests_${PROJECT_NAME})
        add_dependencies(build_all_tests build_tests_${PROJECT_NAME})
    endif()
    
    # Combined targets
    if(NOT TARGET build_all)
        add_custom_target(build_all
            DEPENDS build_all_externals build_all_tests
            COMMENT "Building all externals and tests"
        )
    endif()
    
    if(NOT TARGET build_and_run_all_tests)
        add_custom_target(build_and_run_all_tests
            DEPENDS build_all_tests run_all_tests
            COMMENT "Building and running all tests"
        )
    endif()
endfunction()