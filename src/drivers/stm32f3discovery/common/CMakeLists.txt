# List of source files
set(DRIVERS_COMMON_SOURCES
    IrqHandlers.cpp
    Stm32f3CriticalSectionGuard.cpp
    SysTickImpl.cpp 
)

# Add these files to a library
add_library(drivers_common ${DRIVERS_COMMON_SOURCES})

# Other libraries needed
target_link_libraries(drivers_common 
    PRIVATE 
        common
        interfaces
    PUBLIC
        stm    
)

# Include directories
target_include_directories(drivers_common
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
