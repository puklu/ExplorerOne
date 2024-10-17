# List of source files
set(SYS_STM32F3_SOURCES
    syscalls.c
    sysmem.c
)

# Add these files to a library
add_library(sys_stm32f3 STATIC ${SYS_STM32F3_SOURCES})

# Other libraries needed
target_link_libraries(sys_stm32f3 
    PRIVATE 
)

# Add the linker options to use --whole-archive
target_link_options(sys_stm32f3
    INTERFACE
        -Wl,--whole-archive $<TARGET_FILE:sys_stm32f3> -Wl,--no-whole-archive
)

