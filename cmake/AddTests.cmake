

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
# AUTOMATIC TEST REGISTRATION WITH CTEST
#############################################################

enable_testing()

if(DEFINED PROJECT_LIBRARIES)
    message("-- Linking Project Libraries: ${PROJECT_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_LIBRARIES})
else()
    message("THERE ARE NO PROJECT LIBRARIES DEFINED!")
endif()

# Commented out, we probably don't need this.
if(DEFINED PROJECT_LIBRARIES_SHARED)
    message("-- Linking Shared Project Libraries: ${PROJECT_SHARED_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_LIBRARIES_SHARED})
else()
    message("THERE ARE NO SHARED PROJECT LIBRARIES DEFINED!")
endif()

#############################################################
# TEST SETUP
#############################################################

add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND $<TARGET_FILE:${PROJECT_NAME}_test>
)

set_tests_properties(${PROJECT_NAME}_test 
    PROPERTIES 
        TIMEOUT 30 
        LABELS "unit;shared"
)

message(STATUS "${PROJECT_NAME} test target created and registered with CTest")
