# List of source files
set(I2C_SOURCES
    I2c.cpp
    ServiceISRs.cpp
)

# Add these files to a library
add_library(i2c ${I2C_SOURCES})

# Other libraries needed
target_link_libraries(i2c 
    PRIVATE 
        common
        drivers_common
        interfaces
    PUBLIC
        stm    
)

# Include directories
target_include_directories(i2c
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
