# List of source files
set(COMMON_SOURCES
    assertHandler.cpp
)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Add these files to a library
add_library(common ${COMMON_SOURCES})

# Include directories
target_include_directories(common 
                           PUBLIC 
                           ${SRC_DIR}
)
