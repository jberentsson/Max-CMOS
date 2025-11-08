# Function to find and set MAX_LIBS if not defined.
function(set_max_libs)
    if(DEFINED MAX_LIBS)
        message(STATUS "MAX_LIBS already defined: ${MAX_LIBS}")
        return()
    endif()
    
    message(STATUS "Setting up MAX_LIBS from repository root...")
    
    # Check if the directory exists
    if(NOT EXISTS ${MAX_SDK_BASE_PATH})
        message(WARNING "max-sdk-base not found at: ${MAX_SDK_BASE_PATH}")
        return()
    endif()
    
    # Expected library paths
    set(MAX_API_LIB "${MAX_SDK_BASE_PATH}/c74support/max-includes/x64/MaxAPI.lib")
    set(MAX_AUDIO_LIB "${MAX_SDK_BASE_PATH}/c74support/msp-includes/x64/MaxAudio.lib") 
    set(JIT_LIB "${MAX_SDK_BASE_PATH}/c74support/jit-includes/x64/jitlib.lib")
    
    # Check if libraries exist
    set(FOUND_LIBS)
    
    if(EXISTS ${MAX_API_LIB})
        list(APPEND FOUND_LIBS ${MAX_API_LIB})
        message(STATUS "Found MaxAPI.lib")
    else()
        message(WARNING "MaxAPI.lib not found at: ${MAX_API_LIB}")
    endif()
    
    if(EXISTS ${MAX_AUDIO_LIB})
        list(APPEND FOUND_LIBS ${MAX_AUDIO_LIB})
        message(STATUS "Found MaxAudio.lib")
    else()
        message(WARNING "MaxAudio.lib not found at: ${MAX_AUDIO_LIB}")
    endif()
    
    if(EXISTS ${JIT_LIB})
        list(APPEND FOUND_LIBS ${JIT_LIB})
        message(STATUS "Found jitlib.lib")
    else()
        message(WARNING "jitlib.lib not found at: ${JIT_LIB}")
    endif()
    
    # Set MAX_LIBS if we found libraries
    if(FOUND_LIBS)
        set(MAX_LIBS ${FOUND_LIBS} PARENT_SCOPE)
        message(STATUS "MAX_LIBS set from repository root: ${FOUND_LIBS}")
    else()
        message(FATAL_ERROR "No Max libraries found in repository root")
    endif()
endfunction()
