# List of source files
set(MCU_SOURCES
    mcuInit.cpp
)

# Add these files to a library
add_library(mcu ${MCU_SOURCES})

# Other libraries needed
target_link_libraries(mcu 
    PRIVATE
        common
        interfaces
        stm
)

# Include directories
target_include_directories(mcu
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_CURRENT_SOURCE_DIR}
)
