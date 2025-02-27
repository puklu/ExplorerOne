# List of source files
set(IO_SOURCES
    ExtiPin.cpp
    GpioPin.cpp
    ServiceISRs.cpp
)

# Add these files to a library
add_library(io ${IO_SOURCES})

# Other libraries needed
target_link_libraries(io 
    PRIVATE 
        common
        interfaces
        leds
    PUBLIC
        stm    
)

# Include directories
target_include_directories(io
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
