# List of source files
set(INTERFACES_SOURCES
    IPin.cpp
    PinBase.cpp
)

# Add these files to a library
add_library(interfaces ${INTERFACES_SOURCES})

# Other libraries needed
target_link_libraries(interfaces 
    PRIVATE 
        common
        io
        printf
        usart
    PUBLIC
        stm    
)

# Include directories
target_include_directories(interfaces
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
