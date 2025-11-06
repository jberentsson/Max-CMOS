cmake_minimum_required(VERSION 3.19)

# Extract package name from directory path
string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

message("=== Configuring ${THIS_PACKAGE_NAME} ===")
message("Project: ${THIS_PACKAGE_NAME}")
message("Project Root: ${PROJECT_ROOT}")
message("Build Type: RELEASE (Main targets only)")

# =============================================================================
# REUSABLE FUNCTIONS
# =============================================================================

# Function to create combined test targets
function(create_combined_test_targets PROJECT_NAME)
    message(STATUS "Creating combined test targets for: ${PROJECT_NAME}")
    
    # Build all tests
    if(NOT TARGET build_tests_${PROJECT_NAME})
        add_custom_target(build_tests_${PROJECT_NAME}
            COMMENT "Building all tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Run all tests - CONTINUE ON TEST FAILURES
    if(NOT TARGET run_tests_${PROJECT_NAME})
        add_custom_target(run_tests_${PROJECT_NAME}
            COMMENT "Running all tests for ${PROJECT_NAME} (continues on failure)"
        )
    endif()
    
    # Find all test targets for this project and add them as dependencies
    get_cmake_property(ALL_TARGETS BUILDSYSTEM_TARGETS)
    foreach(target IN LISTS ALL_TARGETS)
        if(target MATCHES "^test_${PROJECT_NAME}_" AND TARGET ${target})
            add_dependencies(build_tests_${PROJECT_NAME} ${target})
            message(STATUS "  - Added ${target} to build_tests_${PROJECT_NAME}")
            
            # Add test execution command
            add_custom_command(TARGET run_tests_${PROJECT_NAME} 
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Running ${target}..."
                COMMAND $<TARGET_FILE:${target}>
                COMMENT "Running ${target}"
            )
        endif()
    endforeach()
    
    # Make run_tests depend on build_tests to ensure tests are built before running
    add_dependencies(run_tests_${PROJECT_NAME} build_tests_${PROJECT_NAME})
endfunction()

# Function to automatically set up Catch2 tests for a project
function(setup_catch2_tests PROJECT_NAME PROJECT_DIR)
    set(CATCH_TEST_SOURCE "${PROJECT_DIR}/tests")
    
    # Check if test directory exists
    if(NOT IS_DIRECTORY ${CATCH_TEST_SOURCE})
        message(STATUS "No test directory found for ${PROJECT_NAME} at ${CATCH_TEST_SOURCE}")
        return()
    endif()
    
    # Find all Catch2 test files dynamically with auto-reconfiguration
    file(GLOB CATCH_TEST_SOURCES CONFIGURE_DEPENDS "${CATCH_TEST_SOURCE}/*_test.cpp")
    
    if(CATCH_TEST_SOURCES AND CATCH2_INCLUDE_DIR)
        message(STATUS "Found Catch2 test sources for ${PROJECT_NAME}: ${CATCH_TEST_SOURCES}")

        foreach(TEST_PATH ${CATCH_TEST_SOURCES})
            # Get just the filename from the path
            get_filename_component(TEST_FILENAME ${TEST_PATH} NAME)
            
            # Remove "_test.cpp" from the filename
            string(REGEX REPLACE "_test\\.cpp$" "" CLEAN_TEST_NAME ${TEST_FILENAME})
            
            message(STATUS "Processing test: ${CLEAN_TEST_NAME} from ${TEST_PATH}")
            
            # Create test executable
            set(TEST_TARGET_NAME "test_${PROJECT_NAME}_${CLEAN_TEST_NAME}")
            add_executable(${TEST_TARGET_NAME} ${TEST_PATH})
            
            target_include_directories(${TEST_TARGET_NAME} PRIVATE
                ${CATCH2_INCLUDE_DIR}
                ${C74_INCLUDES}
                ${PROJECT_DIR}
            )
            
            # Link against the main project
            target_link_libraries(${TEST_TARGET_NAME} PRIVATE ${PROJECT_NAME})
            
            # Add Catch2 compilation definitions
            target_compile_definitions(${TEST_TARGET_NAME} PRIVATE
                CATCH_CONFIG_MAIN  # This tells Catch2 to provide main()
            )
            
            if(WIN32)
                target_link_libraries(${TEST_TARGET_NAME} PRIVATE ws2_32 winmm)
            endif()
            
            add_test(NAME ${PROJECT_NAME}_${CLEAN_TEST_NAME} COMMAND ${TEST_TARGET_NAME})
            
            message(STATUS "Created Catch2 test target: ${TEST_TARGET_NAME}")
        endforeach()
    else()
        if(NOT CATCH2_INCLUDE_DIR)
            message(STATUS "Catch2 not available - tests for ${PROJECT_NAME} will be skipped")
        else()
            message(STATUS "No Catch2 tests found in ${CATCH_TEST_SOURCE} directory")
        endif()
    endif()
endfunction()

# =============================================================================
# MIN-API SETUP
# =============================================================================

message("=== SETTING UP MIN-API ===")

# Search for min-api in possible locations
set(POSSIBLE_MIN_API_PATHS
    "${PROJECT_ROOT}/source/min-api/source/min-api"
    "${PROJECT_ROOT}/min-api"
    "$ENV{MIN_API_PATH}"
)

set(C74_MIN_API_DIR "")
foreach(path ${POSSIBLE_MIN_API_PATHS})
    if(EXISTS "${path}/script/min-pretarget.cmake")
        set(C74_MIN_API_DIR "${path}")
        set(C74_MIN_API_INCLUDE_DIR "${C74_MIN_API_DIR}/include")
        message("-- Found Min-API at: ${C74_MIN_API_DIR}")
        message("-- Found Min-API include files at: ${C74_MIN_API_INCLUDE_DIR}")
        break()
    endif()
endforeach()

if(NOT C74_MIN_API_DIR)
    message(FATAL_ERROR "Min-API not found! Expected at: ${PROJECT_ROOT}/min-api/")
endif()

# Initialize Min-API build system
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# =============================================================================
# CATCH2 TESTING FRAMEWORK SETUP
# =============================================================================

message("=== SETTING UP CATCH2 FOR TESTING ===")

# Check for Catch2 in various locations
set(POSSIBLE_CATCH2_PATHS
    "${PROJECT_ROOT}/source/catch2/catch.hpp"
    "${PROJECT_ROOT}/source/catch.hpp"
    "${PROJECT_ROOT}/catch2/single_include/catch2/catch.hpp"
    "${PROJECT_ROOT}/external/catch2/catch.hpp"
)

# Catch2
set(CATCH2_INCLUDE_DIR "")
foreach(catch_path ${POSSIBLE_CATCH2_PATHS})
    if(EXISTS "${catch_path}")
        get_filename_component(CATCH2_DIR "${catch_path}" DIRECTORY)
        set(CATCH2_INCLUDE_DIR "${CATCH2_DIR}")
        message("-- Found Catch2 at: ${CATCH2_INCLUDE_DIR}")
        break()
    endif()
endforeach()

if(CATCH2_INCLUDE_DIR)
    list(APPEND C74_INCLUDES ${CATCH2_INCLUDE_DIR})
    message("-- Catch2 include directory added: ${CATCH2_INCLUDE_DIR}")
endif()

# =============================================================================
# MAX EXTERNALS (RELEASE ONLY)
# =============================================================================

message("=== BUILDING MAX EXTERNALS (Release) ===")

# Enable testing framework
enable_testing()

# Build all projects in the source/projects directory
file(GLOB PROJECT_DIRS "${PROJECT_ROOT}/source/projects/*")
foreach(project_dir ${PROJECT_DIRS})
    if(IS_DIRECTORY ${project_dir} AND EXISTS "${project_dir}/CMakeLists.txt")
        get_filename_component(PROJECT_NAME ${project_dir} NAME)
        message("Building: ${PROJECT_NAME}")
        add_subdirectory(${project_dir})
        
        # Auto-setup Catch2 tests for this project
        setup_catch2_tests(${PROJECT_NAME} ${project_dir})
        
        # Create combined test targets for this project
        create_combined_test_targets(${PROJECT_NAME})
    endif()
endforeach()

# =============================================================================
# GLOBAL TEST TARGETS - FIXED VERSION
# =============================================================================

# Create global test targets
if(NOT TARGET build_all)
    add_custom_target(build_all
        COMMENT "Building all targets (externals + tests)"
    )
endif()

if(NOT TARGET test_all)
    add_custom_target(test_all
        COMMENT "Running all tests (continues on failure)"
    )
endif()

# Add all main project targets and test targets to build_all
file(GLOB PROJECT_DIRS "${PROJECT_ROOT}/source/projects/*")
foreach(project_dir ${PROJECT_DIRS})
    if(IS_DIRECTORY ${project_dir})
        get_filename_component(PROJECT_NAME ${project_dir} NAME)
        
        # Add main external target
        if(TARGET ${PROJECT_NAME})
            add_dependencies(build_all ${PROJECT_NAME})
            message(STATUS "Added main external: ${PROJECT_NAME} to build_all")
        endif()
        
        # Add ALL individual test targets for this project
        get_cmake_property(ALL_TARGETS BUILDSYSTEM_TARGETS)
        foreach(target IN LISTS ALL_TARGETS)
            if(target MATCHES "^test_${PROJECT_NAME}_" AND TARGET ${target})
                add_dependencies(build_all ${target})
                message(STATUS "Added test executable: ${target} to build_all")
            endif()
        endforeach()
        
        # Add project-specific test run target to test_all
        if(TARGET run_tests_${PROJECT_NAME})
            add_dependencies(test_all run_tests_${PROJECT_NAME})
            message(STATUS "Added test run target: run_tests_${PROJECT_NAME} to test_all")
        endif()
    endif()
endforeach()

# Main build and test target
if(NOT TARGET build_and_test_all)
    add_custom_target(build_and_test_all
        COMMAND ${CMAKE_COMMAND} -E echo "=== Building all targets ==="
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target build_all --config $<CONFIG>
        COMMAND ${CMAKE_COMMAND} -E echo "=== Running all tests ==="
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test_all --config $<CONFIG>
        COMMENT "Building all targets and running all tests (continues on test failures)"
        USES_TERMINAL
    )
endif()

message(STATUS "=== TARGETS CREATED ===")
message(STATUS "Available targets:")
message(STATUS "  build_all - Build all externals and tests")
message(STATUS "  test_all - Run all tests")
message(STATUS "  build_and_test_all - Build everything and run all tests")
message(STATUS "  build_tests_<project> - Build tests for specific project") 
message(STATUS "  run_tests_<project> - Run tests for specific project")
message(STATUS "  test_<project>_<testname> - Individual test executables")