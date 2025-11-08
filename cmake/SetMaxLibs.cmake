# FindMaxLibs.cmake - Robust Max library discovery

#function(set_max_libs)
# Direct approach - use the exact path we know works
set(KNOWN_MAXAPI_PATH "${PROJECT_ROOT}/source/min-api/max-sdk-base/c74support/max-includes/x64/MaxAPI.lib")

if(EXISTS ${KNOWN_MAXAPI_PATH})
    set(MAXAPI_LIB ${KNOWN_MAXAPI_PATH})
    message(STATUS "✅ Using known MaxAPI Library path: ${MAXAPI_LIB}")
else()
    message(FATAL_ERROR "❌ MaxAPI library not found at expected location: ${KNOWN_MAXAPI_PATH}")
endif()
#endfunction()
