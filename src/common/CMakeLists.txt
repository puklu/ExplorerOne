# List of source files
set(COMMON_SOURCES
    assertHandler.cpp
    ringBuffer.cpp
    Trace.cpp
)

# Add these files to a library
add_library(common ${COMMON_SOURCES})

# Other libraries needed
target_link_libraries(common 
    PRIVATE 
        printf                      
        stm
)

# Include directories
target_include_directories(common 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/src
)
