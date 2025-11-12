#############################################################
# Setup
#############################################################

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force STATIC runtime library for ALL targets
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    
    # Force static runtime for ALL configurations
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd ${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "Debug flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT ${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Release flags" FORCE)
    set(CMAKE_C_FLAGS_DEBUG "/MTd ${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "C Debug flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "/MT ${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "C Release flags" FORCE)
endif()

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Extract package name from directory path.
string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)

if(MSVC)
    # Modern way to set runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    
    # Force it for all targets
    add_compile_options(
        "$<$<CONFIG:Debug>:/MDd>"
        "$<$<CONFIG:Release>:/MD>"
        "$<$<CONFIG:RelWithDebInfo>:/MD>"
        "$<$<CONFIG:MinSizeRel>:/MD>"
    )
endif()

#############################################################
# Functions
#############################################################

macro(project_name)
    # Creates the name of the package from the directory name.
    string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
    project(${THIS_PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)
endmacro()

include(${PROJECT_ROOT}/cmake/ProjectTemplate.cmake)
include(${PROJECT_ROOT}/cmake/LibraryTemplate.cmake)

# Enable testing framework.
enable_testing()

# Ignore Test Files
list(FILTER SHARED_SOURCES EXCLUDE REGEX ".*_test\\.cpp$")
set(PROJECT_LIBRARIES_SHARED Catch2::Catch2WithMain)

if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
            if (${XCODE_VERSION} VERSION_LESS 10)
                message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
                return ()
            elseif(${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
                set(C74_BUILD_FAT YES)
            endif ()
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if(C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif()
endif()

# Misc setup and subroutines
include(${CMAKE_CURRENT_SOURCE_DIR}/source/min-api/script/min-package.cmake)

# Add the Lib, if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
endif ()


#############################################################
# CATCH2 SETUP
#############################################################
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found, fetching from GitHub...")
    
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
    )
    
    # Configure Catch2 as header-only to avoid linking issues
    set(CATCH_INSTALL_DOCS OFF CACHE BOOL "")
    set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "")
    set(BUILD_TESTING OFF CACHE BOOL "")
    
    FetchContent_MakeAvailable(Catch2)
endif()

#############################################################
# CATCH2 TESTS FOR SHARED LIBRARIES WITHOUT MAX/MIN
#############################################################

# Generate a project for every folder in the "source/shared" folder

SUBDIRLIST(SHARED_DIRS ${PROJECT_ROOT}/source/shared)

foreach (project_dir ${SHARED_DIRS})
    if (EXISTS "${PROJECT_ROOT}/source/shared/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${PROJECT_ROOT}/source/shared/${project_dir})
    endif ()
endforeach ()

# Comment the line below if you want automatic cmake regeneration enabled.
set(CMAKE_SUPPRESS_REGENERATION true)

#############################################################
# Max
#############################################################

if(WIN32)
    # Generate a project for every folder in the "source/projects" folder
    SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
    foreach (project_dir ${PROJECT_DIRS})
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
            message("Generating: ${project_dir}")
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
        endif ()
    endforeach ()
endif()



if(MSVC)
    # Remove duplicate runtime flags
    foreach(var CXX_FLAGS_DEBUG CXX_FLAGS_RELEASE C_FLAGS_DEBUG C_FLAGS_RELEASE)
        string(REGEX REPLACE "/MTd /MTd+" "/MTd" CMAKE_${var} "${CMAKE_${var}}")
        string(REGEX REPLACE "/MT /MT+" "/MT" CMAKE_${var} "${CMAKE_${var}}")
    endforeach()
endif()
