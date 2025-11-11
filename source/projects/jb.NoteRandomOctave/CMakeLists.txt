cmake_minimum_required(VERSION "3.19")
set(PROJECT_LIBRARIES
    Keyboard
)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

set(PROJECT_NAME jb.NoteRandomOctave)

# Build info variables
set(PROJECT_VERSION "1.0.0")
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S") 
string(TIMESTAMP BUILD_DATETIME "%Y-%m-%d %H:%M:%S")

# FIX THE PATHS - use correct shared directory location
include_directories( 
    ${C74_INCLUDES}
    #${CMAKE_CURRENT_SOURCE_DIR}/../../shared/keyboard
    ${PROJECT_ROOT}/source/shared
)

# FIX THE SOURCE FILE PATHS
set(SOURCE_FILES
    ${PROJECT_NAME}.cpp
    ${PROJECT_NAME}.hpp
    ${PROJECT_ROOT}/source/shared/Keyboard/Keyboard.cpp
    ${PROJECT_ROOT}/source/shared/Keyboard/Keyboard.hpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../../shared/keyboard/Keyboard.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../../shared/keyboard/Keyboard.hpp
)

# CREATE THE MAIN TARGET
add_library(${PROJECT_NAME} MODULE 
            ${SOURCE_FILES}
)

if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
    foreach(PT ${PROJECT_LIBRARIES})
        target_link_libraries(${PROJECT_NAME} ${PL})
    endforeach()
endif()

# Set compile definitions for the main target
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        PROJECT_VERSION=\"${PROJECT_VERSION}\"
        BUILD_DATE=\"${BUILD_DATE}\"
        BUILD_TIME=\"${BUILD_TIME}\"
        BUILD_DATETIME=\"${BUILD_DATETIME}\"
        GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"
)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)

# TODO: Fix this linking error.
if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
    foreach(PT ${PROJECT_LIBRARIES}_test)
        target_link_libraries(${PROJECT_NAME} ${PL})
    endforeach()
endif()
