# Function to find and set MAX_LIBS if not defined
function(set_max_libs)
    # If MAX_LIBS is already set, return early
    if(DEFINED MAX_LIBS)
        message(STATUS "MAX_LIBS already defined: ${MAX_LIBS}")
        return()
    endif()
    
    message(STATUS "MAX_LIBS not defined - searching for Max libraries...")
    
    # Common paths to search for Max libraries
    set(SEARCH_PATHS
        "${C74_MIN_API_DIR}/../lib"
        "${C74_MIN_API_DIR}/../x64/lib"
        "${C74_MIN_API_DIR}/lib"
        "${C74_INCLUDES}/../lib"
        "${C74_INCLUDES}/../x64/lib"
        "${C74_INCLUDES}/lib"
        "$ENV{MAX_ROOT}/lib"
        "$ENV{MAX_ROOT}/x64/lib"
    )
    
    # Platform-specific library names and extensions
    if(WIN32)
        set(LIB_NAMES MaxAPI MaxAudio)
        set(LIB_EXT ".lib")
    else()
        set(LIB_NAMES MaxAPI MaxAudio)
        set(LIB_EXT ".a")
    endif()
    
    # Find each library
    set(FOUND_LIBS)
    foreach(LIB_NAME IN LISTS LIB_NAMES)
        find_library(MAX_LIB_${LIB_NAME}
            NAMES ${LIB_NAME}${LIB_EXT} ${LIB_NAME}
            PATHS ${SEARCH_PATHS}
            NO_DEFAULT_PATH
        )
        
        if(MAX_LIB_${LIB_NAME})
            message(STATUS "Found Max library: ${MAX_LIB_${LIB_NAME}}")
            list(APPEND FOUND_LIBS ${MAX_LIB_${LIB_NAME}})
        else()
            message(WARNING "Could not find Max library: ${LIB_NAME}")
        endif()
    endforeach()
    
    # Set MAX_LIBS in parent scope
    if(FOUND_LIBS)
        set(MAX_LIBS ${FOUND_LIBS} PARENT_SCOPE)
        message(STATUS "MAX_LIBS set to: ${FOUND_LIBS}")
    else()
        message(WARNING "No Max libraries found - MAX_LIBS remains undefined")
    endif()
endfunction()
