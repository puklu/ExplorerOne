# List of source files
set(IO_SOURCES
    GPIOpin.cpp
    IrqHandlers.cpp
    ServiceISRs.cpp
)

# Add these files to a library
add_library(io ${IO_SOURCES})

# Other libraries needed
target_link_libraries(io 
    PRIVATE 
        common
        leds
    PUBLIC
        stm    
)

# Include directories
target_include_directories(io
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
