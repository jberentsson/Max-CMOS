#############################################################
# CATCH2 SETUP
#############################################################

find_path(CATCH2_INCLUDE_DIR
    NAMES catch.hpp
    PATHS 
        ${ROOT_DIR}/source/c74_min_dep/catch2/single_include
        ${ROOT_DIR}/include/catch2/single_include
)

if(NOT CATCH2_INCLUDE_DIR)
    message(STATUS "Catch2 not found locally, downloading...")
    
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/releases/download/v2.13.8/catch.hpp
        DOWNLOAD_NO_EXTRACT TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    set(CATCH2_INCLUDE_DIR ${catch2_SOURCE_DIR})
endif()
