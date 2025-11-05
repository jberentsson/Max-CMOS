# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME jb.CD4024)
set(MAIN_SOURCE ${PROJECT_NAME}.cpp)
set(TEST_SOURCE c74_tests/${PROJECT_NAME}_test.cpp)

# =============================================================================
# MAIN MAX EXTERNAL TARGET
# =============================================================================

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE ${C74_INCLUDES} .)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

# =============================================================================
# UNIT TESTS (Fixed version)
# =============================================================================

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE}" AND CATCH2_INCLUDE_DIR)
    message(STATUS "Found unit test source: ${TEST_SOURCE}")
    
    # 1. Create the executable
    add_executable(test_${PROJECT_NAME} ${TEST_SOURCE})
    
    # 2. Set include directories
    target_include_directories(test_${PROJECT_NAME} PRIVATE
        ${CATCH2_INCLUDE_DIR}
        ${C74_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    # 3. Find and link against the main external as a library
    # This ensures all Max API symbols are resolved through the main target
    target_link_libraries(test_${PROJECT_NAME} PRIVATE ${PROJECT_NAME})
    
    # 4. Add platform-specific libraries for Windows
    if(WIN32)
        target_link_libraries(test_${PROJECT_NAME} PRIVATE 
            ws2_32  # Winsock
            winmm   # Windows multimedia
        )
    endif()
    
    # 5. Add to CTest
    add_test(NAME ${PROJECT_NAME}_tests COMMAND test_${PROJECT_NAME})
    
    message(STATUS "Created test target: test_${PROJECT_NAME}")
else()
    message(STATUS "No unit tests configured - test source not found or Catch2 not available")
endif()