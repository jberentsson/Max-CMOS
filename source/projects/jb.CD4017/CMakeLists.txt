cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME jb.CD4017)
set(MAIN_SOURCE ${PROJECT_NAME}.cpp)

# =============================================================================
# MAIN MAX EXTERNAL TARGET
# =============================================================================

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${C74_INCLUDES} .)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

# =============================================================================
# CATCH2 TESTS
# =============================================================================

set(CATCH_TEST_SOURCE c74_tests/${PROJECT_NAME}_test.cpp)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CATCH_TEST_SOURCE}" AND CATCH2_INCLUDE_DIR)
    message(STATUS "Found Catch2 test source: ${CATCH_TEST_SOURCE}")
    
    add_executable(test_catch_${PROJECT_NAME} ${CATCH_TEST_SOURCE})
    
    target_include_directories(test_catch_${PROJECT_NAME} PRIVATE
        ${CATCH2_INCLUDE_DIR}
        ${C74_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    target_link_libraries(test_catch_${PROJECT_NAME} PRIVATE ${PROJECT_NAME})
    
    if(WIN32)
        target_link_libraries(test_catch_${PROJECT_NAME} PRIVATE ws2_32 winmm)
    endif()
    
    add_test(NAME catch_${PROJECT_NAME} COMMAND test_catch_${PROJECT_NAME})
    
    message(STATUS "Created Catch2 test target: test_catch_${PROJECT_NAME}")
endif()

# =============================================================================
# DOCTEST TESTS
# =============================================================================

setup_doctest_for_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})

# =============================================================================
# COMBINED TEST TARGETS
# =============================================================================

create_combined_test_targets(${PROJECT_NAME})