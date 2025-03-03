# List of source files
set(USART_SOURCES
    ServiceISRs.cpp
    UsartPin.cpp
)

# Add these files to a library
add_library(usart ${USART_SOURCES})

# Other libraries needed
target_link_libraries(usart 
    PRIVATE
        common
        drivers_common
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

# Add the linker options to use --whole-archive
target_link_options(usart
    PRIVATE
        -Wl,--whole-archive $<TARGET_FILE:usart> -Wl,--no-whole-archive
)

