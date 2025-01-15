# List of source files
set(MOTORS_SOURCES
    DriveFactory.cpp
    DriveImpl.cpp
    Motor.cpp
)

# Add these files to a library
add_library(motors ${MOTORS_SOURCES})

# Other libraries needed
target_link_libraries(motors 
    PRIVATE 
        common
        interfaces
    PUBLIC
        stm    
)

# Include directories
target_include_directories(motors
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
