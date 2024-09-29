# List of source files
set(COMMON_SOURCES
    assertHandler.cpp
)

# Add these files to a library
add_library(common ${COMMON_SOURCES})

# Other libraries needed
target_link_libraries(common PRIVATE)

# Include directories
target_include_directories(common 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)
