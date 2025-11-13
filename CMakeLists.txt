cmake_minimum_required(VERSION 3.19)

# FORCE STATIC RUNTIME
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    
    add_compile_options(
        "$<$<CONFIG:Debug>:/MTd>"
        "$<$<CONFIG:Release>:/MT>"
        "$<$<CONFIG:RelWithDebInfo>:/MT>"
        "$<$<CONFIG:MinSizeRel>:/MT>"
    )
    
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
endif()

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Extract package name from directory path.
string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)

#############################################################
# Functions
#############################################################
macro(build_info)
    # Build info variables
    set(PROJECT_VERSION "1.0.0")
    string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
    string(TIMESTAMP BUILD_TIME "%H:%M:%S") 
    string(TIMESTAMP BUILD_DATETIME "%Y-%m-%d %H:%M:%S")
endmacro()


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
# Catch2 - STATIC LIBRARY WITH STATIC RUNTIME
#############################################################

# Save current runtime library settings
set(OLD_CMAKE_MSVC_RUNTIME_LIBRARY ${CMAKE_MSVC_RUNTIME_LIBRARY})

# FORCE STATIC RUNTIME FOR CATCH2
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

#############################################################
# Catch2 - PURE STATIC LIBRARY
#############################################################

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.8
)

# FORCE STATIC LIBRARY
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)

if(MSVC)
    set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
endif()

FetchContent_MakeAvailable(Catch2)

# Restore original runtime settings
set(CMAKE_MSVC_RUNTIME_LIBRARY ${OLD_CMAKE_MSVC_RUNTIME_LIBRARY})

# CREATE TARGETS
if(TARGET Catch2)
    # TODO: Simplify this.
    if(NOT TARGET Catch2::Catch2)
        add_library(Catch2::Catch2 ALIAS Catch2)
    endif()
    if(NOT TARGET Catch2::Catch2WithMain)
        add_library(Catch2::Catch2WithMain ALIAS Catch2)
    endif()
    
    # Force static runtime on the Catch2 target specifically
    if(MSVC)
        set_target_properties(Catch2 PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()
    
    message(STATUS "Catch2 configured as static library with static runtime")
else()
    message(FATAL_ERROR "Catch2 target not found!")
endif()

#############################################################
# CATCH2 TESTS FOR SHARED LIBRARIES WITHOUT MAX/MIN
#############################################################

SUBDIRLIST(SHARED_DIRS ${PROJECT_ROOT}/source/shared)
#set(SHARED_DIRS Exceptions Counter CD4017 CD4024 Keyboard)

# Generate a project for every folder in the "source/shared" folder.
foreach (project_dir ${SHARED_DIRS})
    if (EXISTS "${PROJECT_ROOT}/source/shared/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${PROJECT_ROOT}/source/shared/${project_dir})
    endif ()
endforeach ()

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
