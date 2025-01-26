# List of source files
set(MOTION_SOURCES
    DriveFactory.cpp
    DriveImpl.cpp
    Motor.cpp
)

# Add these files to a library
add_library(motion ${MOTION_SOURCES})

# Other libraries needed
target_link_libraries(motion 
    PRIVATE 
        common
        interfaces
    PUBLIC
        stm    
)

# Include directories
target_include_directories(motion
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
