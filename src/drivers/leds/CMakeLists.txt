# List of source files
set(LED_SOURCES
    leds.cpp
)

# Add these files to a library
add_library(leds ${LED_SOURCES})

# Other libraries needed
target_link_libraries(leds 
    PRIVATE 
        common
        io 
)

# Include directories
target_include_directories(leds
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
