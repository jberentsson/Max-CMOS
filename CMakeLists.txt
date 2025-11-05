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

# Function to setup doctest for a project
function(setup_doctest_for_project PROJECT_NAME PROJECT_DIR)
    message(STATUS "Setting up doctest for: ${PROJECT_NAME}")
    
    # Find doctest directory
    set(DOCTEST_DIR ${PROJECT_DIR}/doctest_tests)
    message(STATUS "Looking for doctest directory: ${DOCTEST_DIR}")
    
    # Find or download doctest
    if(NOT DOCTEST_INCLUDE_DIR)
        find_path(DOCTEST_INCLUDE_DIR
            NAMES doctest.h
            PATHS
                ${CMAKE_SOURCE_DIR}/source/doctest
                ${CMAKE_SOURCE_DIR}/extern/doctest
                ${CMAKE_SOURCE_DIR}/doctest
                /usr/local/include
                /usr/include
        )
    endif()
    
    # Download doctest if not found
    if(NOT DOCTEST_INCLUDE_DIR)
        message(STATUS "doctest not found, downloading...")
        include(FetchContent)
        FetchContent_Declare(
            doctest
            URL https://github.com/doctest/doctest/releases/download/v2.4.11/doctest-2.4.11.zip
        )
        FetchContent_GetProperties(doctest)
        if(NOT doctest_POPULATED)
            FetchContent_Populate(doctest)
            set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR} PARENT_SCOPE)
        endif()
        message(STATUS "✅ Downloaded doctest to: ${DOCTEST_INCLUDE_DIR}")
    else()
        message(STATUS "✅ Found doctest at: ${DOCTEST_INCLUDE_DIR}")
    endif()
    
    # Find all doctest files dynamically
    if(EXISTS ${DOCTEST_DIR} AND DOCTEST_INCLUDE_DIR AND EXISTS "${DOCTEST_INCLUDE_DIR}")
        message(STATUS "✅ Found doctest directory: ${DOCTEST_DIR}")
        file(GLOB DOCTEST_SOURCES "${DOCTEST_DIR}/*.cpp")
        message(STATUS "Found doctest sources: ${DOCTEST_SOURCES}")
        
        # Process each doctest file
        foreach(doctest_source ${DOCTEST_SOURCES})
            get_filename_component(test_name ${doctest_source} NAME_WE)
            
            # Remove "_test" suffix if present for cleaner target names
            string(REGEX REPLACE "_test$" "" clean_test_name ${test_name})
            
            message(STATUS "Found doctest source: ${test_name}.cpp -> target: test_doctest_${clean_test_name}")
            
            # Only create the executable if it doesn't exist
            if(NOT TARGET test_doctest_${clean_test_name})
                add_executable(test_doctest_${clean_test_name} ${doctest_source})
                
                target_include_directories(test_doctest_${clean_test_name} PRIVATE
                    ${DOCTEST_INCLUDE_DIR}
                    ${PROJECT_DIR}
                    ${CMAKE_SOURCE_DIR}/source
                    ${C74_INCLUDES}
                )
                
                target_compile_definitions(test_doctest_${clean_test_name} PRIVATE
                    DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
                )
                
                if(WIN32)
                    target_link_libraries(test_doctest_${clean_test_name} PRIVATE 
                        ws2_32
                        winmm
                    )
                endif()
                
                message(STATUS "✅ Created doctest target: test_doctest_${clean_test_name}")
            else()
                message(STATUS "✅ Doctest target already exists: test_doctest_${clean_test_name}")
            endif()
            
            # Only register the test if it doesn't exist
            get_test_property(test_exists doctest_${clean_test_name} DEFINED)
            if(NOT test_exists)
                add_test(NAME doctest_${clean_test_name} COMMAND test_doctest_${clean_test_name})
                message(STATUS "✅ Registered CTest: doctest_${clean_test_name}")
            else()
                message(STATUS "✅ CTest already registered: doctest_${clean_test_name}")
            endif()
        endforeach()
        
        if(NOT DOCTEST_SOURCES)
            message(STATUS "No doctest sources found in ${DOCTEST_DIR}")
        endif()
    else()
        if(NOT EXISTS ${DOCTEST_DIR})
            message(STATUS "doctest directory not found: ${DOCTEST_DIR}")
        endif()
        if(NOT DOCTEST_INCLUDE_DIR)
            message(STATUS "doctest include directory not found")
        endif()
    endif()
endfunction()

# Function to create combined test targets
function(create_combined_test_targets PROJECT_NAME)
    message(STATUS "Creating combined test targets for: ${PROJECT_NAME}")
    
    # Build all tests (both Catch2 and doctest)
    if(NOT TARGET build_tests_${PROJECT_NAME})
        add_custom_target(build_tests_${PROJECT_NAME}
            COMMENT "Building all tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Run all tests (both Catch2 and doctest) - CONTINUE ON TEST FAILURES
    if(NOT TARGET run_tests_${PROJECT_NAME})
        add_custom_target(run_tests_${PROJECT_NAME}
            COMMENT "Running all tests for ${PROJECT_NAME} (continues on failure)"
        )
    endif()
    
    # Add ALL test executables to build target
    if(TARGET test_catch_${PROJECT_NAME})
        add_dependencies(build_tests_${PROJECT_NAME} test_catch_${PROJECT_NAME})
        message(STATUS "  - Added test_catch_${PROJECT_NAME} to build_tests_${PROJECT_NAME}")
    endif()
    
    # Add all doctest targets to build target
    set(DOCTEST_TARGETS_FOUND FALSE)
    get_cmake_property(ALL_TARGETS GLOBAL PROPERTY BUILDSYSTEM_TARGETS)
    
    foreach(target ${ALL_TARGETS})
        if(target MATCHES "^test_doctest_")
            add_dependencies(build_tests_${PROJECT_NAME} ${target})
            set(DOCTEST_TARGETS_FOUND TRUE)
            message(STATUS "  - Added ${target} to build_tests_${PROJECT_NAME}")
        endif()
    endforeach()
    
    if(DOCTEST_TARGETS_FOUND)
        message(STATUS "✅ Doctest targets found and added for ${PROJECT_NAME}")
    else()
        message(STATUS "No doctest targets found for ${PROJECT_NAME}")
    endif()
    
    # Add explicit run commands using POST_BUILD - CONTINUE ON FAILURE WITH FIXED QUOTING
    if(TARGET test_catch_${PROJECT_NAME})
        add_custom_command(TARGET run_tests_${PROJECT_NAME} 
            POST_BUILD
            COMMAND cmd /c "echo Running Catch2 tests... && \"$<TARGET_FILE:test_catch_${PROJECT_NAME}>\" || echo Catch2 tests completed with status %errorlevel%"
            COMMENT "Running Catch2 tests for ${PROJECT_NAME}"
        )
    endif()
    
    # Add doctest run commands - CONTINUE ON FAILURE WITH FIXED QUOTING
    foreach(target ${ALL_TARGETS})
        if(target MATCHES "^test_doctest_")
            add_custom_command(TARGET run_tests_${PROJECT_NAME} 
                POST_BUILD
                COMMAND cmd /c "echo Running ${target}... && \"$<TARGET_FILE:${target}>\" || echo ${target} completed with status %errorlevel%"
                COMMENT "Running ${target}"
            )
        endif()
    endforeach()
    
    # Make run_tests depend on build_tests to ensure tests are built before running
    add_dependencies(run_tests_${PROJECT_NAME} build_tests_${PROJECT_NAME})
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
        message("✅ Found Min-API at: ${C74_MIN_API_DIR}")
        message("✅ Found Min-API include files at: ${C74_MIN_API_INCLUDE_DIR}")
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
        message("✅ Found Catch2 at: ${CATCH2_INCLUDE_DIR}")
        break()
    endif()
endforeach()

if(CATCH2_INCLUDE_DIR)
    list(APPEND C74_INCLUDES ${CATCH2_INCLUDE_DIR})
    message("✅ Catch2 include directory added: ${CATCH2_INCLUDE_DIR}")
endif()

# =============================================================================
# DOCTEST TESTING FRAMEWORK SETUP
# =============================================================================

message("=== SETTING UP DOCTEST FOR TESTING ===")

# Global doctest setup - download once for all projects
if(NOT DOCTEST_INCLUDE_DIR)
    message(STATUS "Setting up global doctest...")
    include(FetchContent)
    FetchContent_Declare(
        doctest
        URL https://github.com/doctest/doctest/releases/download/v2.4.11/doctest-2.4.11.zip
    )
    FetchContent_GetProperties(doctest)
    if(NOT doctest_POPULATED)
        FetchContent_Populate(doctest)
        set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR})
    endif()
endif()

if(DOCTEST_INCLUDE_DIR AND EXISTS "${DOCTEST_INCLUDE_DIR}")
    message(STATUS "✅ Global doctest setup complete: ${DOCTEST_INCLUDE_DIR}")
endif()

# =============================================================================
# MAX EXTERNALS (RELEASE ONLY)
# =============================================================================

message("=== BUILDING MAX EXTERNALS (Release) ===")

# Build all projects in the source/projects directory
file(GLOB PROJECT_DIRS "${PROJECT_ROOT}/source/projects/*")
foreach(project_dir ${PROJECT_DIRS})
    if(IS_DIRECTORY ${project_dir} AND EXISTS "${project_dir}/CMakeLists.txt")
        get_filename_component(PROJECT_NAME ${project_dir} NAME)
        message("Building: ${PROJECT_NAME}")
        add_subdirectory(${project_dir})
    endif()
endforeach()

# =============================================================================
# GLOBAL TEST TARGETS - CONTINUE ON TEST FAILURES WITH FIXED QUOTING
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

# Add all targets to build_all
file(GLOB PROJECT_DIRS "${PROJECT_ROOT}/source/projects/*")
foreach(project_dir ${PROJECT_DIRS})
    if(IS_DIRECTORY ${project_dir})
        get_filename_component(PROJECT_NAME ${project_dir} NAME)
        
        # Add main external
        if(TARGET ${PROJECT_NAME})
            add_dependencies(build_all ${PROJECT_NAME})
        endif()
        
        # Add all test executables
        get_cmake_property(ALL_TARGETS GLOBAL PROPERTY BUILDSYSTEM_TARGETS)
        foreach(target ${ALL_TARGETS})
            if(target MATCHES "^test_" OR target MATCHES "^test_catch_" OR target MATCHES "^test_doctest_")
                add_dependencies(build_all ${target})
            endif()
        endforeach()
    endif()
endforeach()

# Add test execution commands that continue on failure - WITH FIXED QUOTING
get_cmake_property(ALL_TARGETS GLOBAL PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${ALL_TARGETS})
    if(target MATCHES "^test_catch_")
        add_custom_command(TARGET test_all
            POST_BUILD
            COMMAND cmd /c "echo === Running $<TARGET_FILE_NAME:${target}> === && \"$<TARGET_FILE:${target}>\" || echo $<TARGET_FILE_NAME:${target}> completed with status %errorlevel%"
            COMMENT "Running ${target}"
        )
    elseif(target MATCHES "^test_doctest_")
        add_custom_command(TARGET test_all
            POST_BUILD
            COMMAND cmd /c "echo === Running $<TARGET_FILE_NAME:${target}> === && \"$<TARGET_FILE:${target}>\" || echo $<TARGET_FILE_NAME:${target}> completed with status %errorlevel%"
            COMMENT "Running ${target}"
        )
    endif()
endforeach()

# Main build and test target
if(NOT TARGET build_and_test_all)
    add_custom_target(build_and_test_all
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target build_all
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test_all
        COMMENT "Building all targets and running all tests (continues on test failures)"
        DEPENDS build_all
    )
endif()

message("=== ${THIS_PACKAGE_NAME} Release Configuration Complete ===")