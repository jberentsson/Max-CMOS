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



###################
# DEBUGGING
####################

# Test with different path formats
set(RAW_PATH "${PROJECT_ROOT}/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64")

message("Testing raw path: ${RAW_PATH}")
if(EXISTS "${RAW_PATH}/MaxAPI.lib")
    message("✅ EXISTS with raw path")
else()
    message("❌ DOES NOT EXIST with raw path")
endif()

# Try with escaped characters
string(REPLACE " " "\\ " ESCAPED_PATH "${RAW_PATH}")
message("Testing escaped path: ${ESCAPED_PATH}")
if(EXISTS "${ESCAPED_PATH}/MaxAPI.lib")
    message("✅ EXISTS with escaped path")
else()
    message("❌ DOES NOT EXIST with escaped path")
endif()


# Hardcode the exact path
set(HARDCODED_PATH "${PROJECT_ROOT}/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64")

message("Hardcoded path: ${HARDCODED_PATH}")
if(EXISTS "${HARDCODED_PATH}/MaxAPI.lib")
    message("✅ HARDCODED PATH WORKS!")
    set(C74_MAX_API_X64_DIR "${HARDCODED_PATH}")
else()
    message("❌ Even hardcoded path fails!")
endif()

# Use file GLOB to find the file regardless of path issues
file(GLOB_RECURSE MAX_API_LIB_FILES 
    "C:/Users/Jóhann*/Documents/Max 8/Packages/Max-CMOS/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64/MaxAPI.lib"
)

if(MAX_API_LIB_FILES)
    message("✅ Found via GLOB: ${MAX_API_LIB_FILES}")
    get_filename_component(C74_MAX_API_X64_DIR "${MAX_API_LIB_FILES}" DIRECTORY)
else()
    message("❌ Not found via GLOB")
endif()

cmake_path(SET TEST_PATH "C:/Users/Jóhann Berentsson/Documents/Max 8/Packages/Max-CMOS/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64")
cmake_path(NORMAL_PATH TEST_PATH)

message("Normalized path: ${TEST_PATH}")
if(EXISTS "${TEST_PATH}/MaxAPI.lib")
    message("✅ EXISTS with normalized path!")
    set(C74_MAX_API_X64_DIR "${TEST_PATH}")
endif()




#############################################################
# MIN-API SETUP
#############################################################

message("=== SETTING UP MIN-API ===")





# Search for min-api in possible locations
set(POSSIBLE_MIN_API_PATHS
    "${PROJECT_ROOT}/source/min-api/source/min-api"    # Primary location
    "${PROJECT_ROOT}/min-api"                   # Fallback location
    "$ENV{MIN_API_PATH}"                        # Environment variable
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


# MIN-API Imports

set(C74_MIN_API_INCLUDE_DIR "")
if(EXISTS "${C74_MIN_API_DIR}/include")
    set(C74_MIN_API_INCLUDE_DIR "${C74_MIN_API_DIR}/include")

    message("✅ Found Min-API include files at: ${C74_MIN_API_INCLUDE_DIR}")
endif()

if(NOT C74_MIN_API_DIR)
    message(FATAL_ERROR "Min-API includes not found! Expected at: ${C74_MIN_API_DIR}/min-api/include")
endif()

# MAN-API 

#set(POSSIBLE_MAX_API_PATHS "${C74_MIN_API_DIR}/max-sdk-base/c74support")
#
#set(C74_MAX_API_DIR "")
#if(EXISTS "${POSSIBLE_MAX_API_PATHS}")
#    set(C74_MAX_API_DIR "${POSSIBLE_MAX_API_PATHS}")
#    set(C74_MAX_API_INCLUDE_DIR "${C74_MAX_API_DIR}/max-includes")
#
#    message("✅ Found Max-API files at: ${C74_MAX_API_DIR}")
#endif()
#
#if(NOT C74_MAX_API_DIR)
#    message(FATAL_ERROR "Man-API includes not found! Expected at: ${POSSIBLE_MAX_API_PATHS}")
#endif()


# MaxAPI.lib - with diagnostics
#set(POSSIBLE_MAX_LIB_PATHS
#    "${PROJECT_ROOT}/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64/Release"
#    "${PROJECT_ROOT}/source/min-api/source/min-api/max-sdk-base/c74support/max-includes/x64"
#    "${C74_MIN_API_DIR}/max-sdk-base/lib/x64/Release"
#    "${C74_MIN_API_DIR}/max-sdk-base/lib/x64"
#    "${C74_MIN_API_DIR}/max-sdk-base/lib"
#    "${C74_MIN_API_DIR}/lib/x64"
#    "${C74_MIN_API_DIR}/lib"
#)
#
#message("=== Searching for MaxAPI.lib ===")
#set(C74_MAX_API_X64_DIR "")
#foreach(lib_path ${POSSIBLE_MAX_LIB_PATHS})
#    message("Checking: ${lib_path}/MaxAPI.lib")
#    if(EXISTS "${lib_path}/MaxAPI.lib")
#        set(C74_MAX_API_X64_DIR "${lib_path}")
#        message("✅ FOUND MaxAPI.lib at: ${lib_path}")
#        break()
#    else()
#        message("❌ NOT found at: ${lib_path}")
#    endif()
#endforeach()
#
#if(NOT C74_MAX_API_X64_DIR)
#    message(FATAL_ERROR "MaxAPI.lib not found in any of the checked locations!")
#endif()
#
#message("✅ Using MaxAPI.lib from: ${C74_MAX_API_X64_DIR}")



# Shared Libraries

set(POSSIBLE_SHARED_LIBRARIES "${PROJECT_ROOT}/source/projects/shared_libraries")

set(SHARED_LIBRARIES "")
if(EXISTS "${POSSIBLE_SHARED_LIBRARIES}")
    set(SHARED_LIBRARIES "${POSSIBLE_SHARED_LIBRARIES}")

    message("✅ Found shared library at: ${SHARED_LIBRARIES}")
endif()

if(NOT SHARED_LIBRARIES)
    message(FATAL_ERROR "Shared library not found! Expected at: ${POSSIBLE_SHARED_LIBRARIES}")
endif()



# Initialize Min-API build system
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

message("C74_INCLUDES: ${C74_INCLUDES}")

#############################################################
# CATCH2 TESTING FRAMEWORK SETUP
#############################################################

message("=== SETTING UP CATCH2 FOR TESTING ===")

# Check for Catch2 in various locations
set(POSSIBLE_CATCH2_PATHS
    "${PROJECT_ROOT}/source/catch2/catch.hpp"           # catch.hpp directly in catch2 directory
    "${PROJECT_ROOT}/source/catch.hpp"                  # catch.hpp directly in source
    "${PROJECT_ROOT}/catch2/single_include/catch2/catch.hpp"  # Standard Catch2 structure
    "${PROJECT_ROOT}/external/catch2/catch.hpp"         # External directory
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

# Catch2 Includes
if(NOT CATCH2_INCLUDE_DIR)
    message(WARNING "❌ Catch2 not found - BDD tests (SCENARIO/GIVEN/WHEN/THEN) will not compile")
    message("   Expected locations:")
    foreach(catch_path ${POSSIBLE_CATCH2_PATHS})
        message("   - ${catch_path}")
    endforeach()
else()
    # Add Catch2 include directory to global includes
    list(APPEND C74_INCLUDES ${CATCH2_INCLUDE_DIR})
    message("✅ Catch2 include directory added: ${CATCH2_INCLUDE_DIR}")
endif()

# C74 Unittest
if(EXISTS "${C74_MIN_API_DIR}/include/c74_min_unittest.h")
    message("✅ Found c74_min_unittest.h at: ${C74_MIN_API_DIR}/include/c74_min_unittest.h")
    # Add unittest include directory
    list(APPEND C74_INCLUDES ${C74_MIN_API_DIR}/include)
else()
    message(WARNING "❌ c74_min_unittest.h not found - unit tests may not compile")
endif()

#############################################################
# MAX EXTERNALS (RELEASE ONLY)
#############################################################

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

message("=== ${THIS_PACKAGE_NAME} Release Configuration Complete ===")



#####################################

