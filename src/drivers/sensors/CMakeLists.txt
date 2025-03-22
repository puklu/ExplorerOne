# List of source files
set(SENSORS_SOURCES
    UltrasonicSensor.cpp
)

# Add these files to a library
add_library(sensors ${SENSORS_SOURCES})

# Other libraries needed
target_link_libraries(sensors 
    PRIVATE 
        common
        interfaces
    PUBLIC
        stm    
)

# Include directories
target_include_directories(sensors
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
