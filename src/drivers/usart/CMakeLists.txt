# List of source files
set(USART_SOURCES
    IrqHandlers.cpp
    UsartPin.cpp
)

# Add these files to a library
add_library(usart ${USART_SOURCES})

# Other libraries needed
target_link_libraries(usart 
    PRIVATE
        common
        interfaces
        io
        printf
    PUBLIC
        stm    
)

# Include directories
target_include_directories(usart
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
