# List of source files
set(COMMON_SOURCES
    assertHandler.cpp
)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Add these files to a library
add_library(common ${COMMON_SOURCES})

# Needs drivers library
target_link_libraries(common PRIVATE drivers)

# Include directories
target_include_directories(common 
                           PUBLIC 
                           ${SRC_DIR}
)
