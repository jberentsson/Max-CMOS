# FindMaxLibs.cmake - Robust Max library discovery

function(set_max_libs)
    if(DEFINED MAX_LIBS)
        return()
    endif()
    
    # Your reliable library discovery logic here
    set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    set(MAX_SDK_BASE_PATH "${REPO_ROOT}/max-sdk-base")
   
    message("Max SDK base path: ${MAX_SDK_BASE_PATH}")

    if(EXISTS "${MAX_SDK_BASE_PATH}")
        set(MAX_API_LIB "${MAX_SDK_BASE_PATH}/c74support/max-includes/x64/MaxAPI.lib")
        set(MAX_AUDIO_LIB "${MAX_SDK_BASE_PATH}/c74support/msp-includes/x64/MaxAudio.lib") 
        set(JIT_LIB "${MAX_SDK_BASE_PATH}/c74support/jit-includes/x64/jitlib.lib")
        
        set(FOUND_LIBS)
        foreach(LIB IN ITEMS ${MAX_API_LIB} ${MAX_AUDIO_LIB} ${JIT_LIB})
            if(EXISTS "${LIB}")
                list(APPEND FOUND_LIBS "${LIB}")
            endif()
        endforeach()
        
        if(FOUND_LIBS)
            set(MAX_LIBS ${FOUND_LIBS} PARENT_SCOPE)
            set(MAX_SDK_BASE_PATH ${MAX_SDK_BASE_PATH} PARENT_SCOPE)
        endif()
    endif()
endfunction()
