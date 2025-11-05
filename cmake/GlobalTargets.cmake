# Function to create combined test targets
function(create_combined_test_targets PROJECT_NAME)
    message(STATUS "Creating combined test targets for: ${PROJECT_NAME}")
    
    # Build all doctest tests only
    if(NOT TARGET build_only_doctest_${PROJECT_NAME})
        add_custom_target(build_only_doctest_${PROJECT_NAME}
            COMMENT "Building only doctest tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Run all doctest tests only  
    if(NOT TARGET run_only_doctest_${PROJECT_NAME})
        add_custom_target(run_only_doctest_${PROJECT_NAME}
            COMMENT "Running only doctest tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Dynamically add all doctest targets to separate doctest targets
    get_cmake_property(ALL_TARGETS GLOBAL PROPERTY BUILDSYSTEM_TARGETS)
    set(DOCTEST_TARGETS_FOUND FALSE)
    
    foreach(target ${ALL_TARGETS})
        if(target MATCHES "^test_doctest_")
            # Extract the test name from the target
            string(REGEX REPLACE "^test_doctest_" "" test_name ${target})
            
            message(STATUS "Found doctest target: ${target} -> ${test_name}")
            
            # Add to build targets
            add_dependencies(build_only_doctest_${PROJECT_NAME} ${target})
            
            # Add to run targets - create a custom command to run the doctest
            add_custom_command(TARGET run_only_doctest_${PROJECT_NAME} 
                POST_BUILD
                COMMAND $<TARGET_FILE:${target}>
                COMMENT "Running ${target}"
            )
            
            set(DOCTEST_TARGETS_FOUND TRUE)
            message(STATUS "  - Added ${target} to doctest targets")
        endif()
    endforeach()
    
    if(DOCTEST_TARGETS_FOUND)
        message(STATUS "✅ Doctest targets successfully added to separate targets")
    else()
        message(STATUS "No doctest targets found")
    endif()
    
    # Build all tests (both Catch2 and doctest)
    if(NOT TARGET build_tests_${PROJECT_NAME})
        add_custom_target(build_tests_${PROJECT_NAME}
            COMMENT "Building all tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Run all tests (both Catch2 and doctest)
    if(NOT TARGET run_tests_${PROJECT_NAME})
        add_custom_target(run_tests_${PROJECT_NAME}
            COMMENT "Running all tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Add ALL test executables to build target
    if(TARGET test_catch_${PROJECT_NAME})
        add_dependencies(build_tests_${PROJECT_NAME} test_catch_${PROJECT_NAME})
        message(STATUS "  - Added test_catch_${PROJECT_NAME} to build_tests_${PROJECT_NAME}")
    endif()
    
    # Add all doctest targets to build target
    foreach(target ${ALL_TARGETS})
        if(target MATCHES "^test_doctest_")
            add_dependencies(build_tests_${PROJECT_NAME} ${target})
            message(STATUS "  - Added ${target} to build_tests_${PROJECT_NAME}")
        endif()
    endforeach()
    
    # Add explicit run commands using POST_BUILD
    if(TARGET test_catch_${PROJECT_NAME})
        add_custom_command(TARGET run_tests_${PROJECT_NAME} 
            POST_BUILD
            COMMAND $<TARGET_FILE:test_catch_${PROJECT_NAME}>
            COMMENT "Running Catch2 tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Add doctest run commands
    foreach(target ${ALL_TARGETS})
        if(target MATCHES "^test_doctest_")
            add_custom_command(TARGET run_tests_${PROJECT_NAME} 
                POST_BUILD
                COMMAND $<TARGET_FILE:${target}>
                COMMENT "Running ${target}"
            )
        endif()
    endforeach()
    
    # Make run_tests depend on build_tests to ensure tests are built before running
    add_dependencies(run_tests_${PROJECT_NAME} build_tests_${PROJECT_NAME})
endfunction()