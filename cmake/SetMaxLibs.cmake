# FindMaxLibs.cmake - Robust Max library discovery

function(set_max_libs)
    if(DEFINED MAX_LIBS)
        return()
    endif()
    
    # Your reliable library discovery logic here
    set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    set(MAX_SDK_BASE_PATH "${REPO_ROOT}/max-sdk-base")
    
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

function(copy_max_libs)
    if(WIN32)
        # The DLLs we need to copy
        set(MAX_API_DLL "${MAX_SDK_BASE_PATH}/c74support/max-includes/x64/MaxAPI.dll")
        set(MAX_AUDIO_DLL "${MAX_SDK_BASE_PATH}/c74support/msp-includes/x64/MaxAudio.dll")

        message("MAX_API_DLL: ${MAX_SDK_BASE_PATH}")
        message("MAX_AUDIO: ${MAX_SDK_BASE_PATH}")

        set(LIBRARY_DESTINATION  "${PROJECT_ROOT}/build/source/projects/${PROJECT_NAME}/Release/${PROJECT_NAME}_test.exp")

        message("LIBRARY DESTINATION: ${LIBRARY_DESTINATION}")

        # Copy MaxAPI.dll
        add_custom_command(TARGET ${LIBRARY_DESTINATION}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MAX_API_DLL}"
            $LIBRARY_DESTINATION
            COMMENT "Copying MaxAPI.dll to test executable directory"
        )

        # Copy MaxAudio.dll
        add_custom_command(TARGET "${LIBRARY_DESTINATION}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MAX_AUDIO_DLL}"
            $LIBRARY_DESTINATION
            COMMENT "Copying MaxAudio.dll to test executable directory"
        )

        # Similarly, if there's a jitlib.dll, copy it too.
        # set(JIT_DLL "${MAX_SDK_BASE_PATH}/c74support/jit-includes/x64/jitlib.dll")
        # add_custom_command(TARGET jb.${PROJECT_NAME}_test POST_BUILD
        #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #     "${JIT_DLL}"
        #     "$<TARGET_FILE_DIR:jb.${PROJECT_NAME}_test>"
        #     COMMENT "Copying jitlib.dll to test executable directory"
        # )
    endif()
endfunction()
