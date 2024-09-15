# List of source files
set(DRIVERS_SOURCES
    io/io.cpp
)

# Add these files to a library
add_library(drivers ${DRIVERS_SOURCES})

# Include directories
target_include_directories(drivers PUBLIC ${CMAKE_SOURCE_DIR}/src/drivers)
