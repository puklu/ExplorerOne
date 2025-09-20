# List of source files
set(FACTORY_SOURCES
    PinFactory.cpp
    TimerFactory.cpp
)

# Add these files to a library
add_library(factory ${FACTORY_SOURCES})

# Other libraries needed
target_link_libraries(factory 
    PRIVATE
        common
        io
        printf
        usart
    PUBLIC
        stm    
)

# Include directories
target_include_directories(factory
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
