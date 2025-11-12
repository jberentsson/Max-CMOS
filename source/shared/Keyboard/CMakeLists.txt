project_name()

# Use static library - no export/import needed
add_library(Keyboard STATIC
    Keyboard.hpp
    Keyboard.cpp
)

target_include_directories(Keyboard PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ActiveNote> 
)

if(WIN32)
    set_target_properties(Keyboard PROPERTIES
        OUTPUT_NAME Keyboard
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared/Keyboard/$<CONFIG>
    )
endif()

# Force static runtime for Keyboard - add this at the end
if(MSVC)
    # Explicitly force static runtime library
    target_compile_options(Keyboard PRIVATE 
        "$<$<CONFIG:Debug>:/MTd>"
        "$<$<CONFIG:Release>:/MT>"
        "$<$<CONFIG:RelWithDebInfo>:/MT>"
        "$<$<CONFIG:MinSizeRel>:/MT>"
    )
    
    # Also set via properties as backup
    #set_target_properties(Keyboard PROPERTIES
    #    MSVC_RUNTIME_LIBRARY "MultiThreaded"
    #)
    
    message(STATUS "Keyboard: Explicitly forced static runtime")
endif()
