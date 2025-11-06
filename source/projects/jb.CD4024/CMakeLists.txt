cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME ${PROJECT_NAME})
set(MAIN_SOURCE ${PROJECT_NAME}.cpp)

# =============================================================================
# MAIN MAX EXTERNAL TARGET
# =============================================================================

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${C74_INCLUDES} .)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)