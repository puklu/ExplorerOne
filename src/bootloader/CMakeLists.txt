# List of source files
set(BOOTLOADER_SOURCES
    main.cpp
)

# Add these files to a library
add_library(bootloader ${BOOTLOADER_SOURCES})

# Other libraries needed
target_link_libraries(bootloader 
    PRIVATE 
        common
        stm
        sys_stm32f3
)

# Include directories
target_include_directories(bootloader
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)

# Startup file
set(BOOTLOADER_START_UP_FILE "${CMAKE_SOURCE_DIR}/src/bootloader/startup_bootloader_stm32f303vctx.s")

# Combine startup file and bootloader library sources for the executable
set(BOOTLOADER_EXEC_SOURCES
    ${BOOTLOADER_START_UP_FILE}
    $<TARGET_PROPERTY:bootloader,SOURCES>
)

# Create the bootloader executable
add_executable(bootloader.elf ${BOOTLOADER_EXEC_SOURCES})

# Link with bootloader library and dependencies
target_link_libraries(bootloader.elf
    PRIVATE
        bootloader  
)

#  Apply bootloader-specific linker script
target_link_options(bootloader.elf
    PRIVATE
        "-T${CMAKE_SOURCE_DIR}/src/bootloader/bootloader.ld"
        "-Wl,--gc-sections"
)

# Tell the linker to generate a map file for bootloader
set_target_properties(bootloader.elf
    PROPERTIES
        LINK_FLAGS "-Wl,-Map=bootloader.map"
        COMMENT "Generating Map file for bootloader"
)

# Convert ELF to binary
add_custom_command(TARGET bootloader.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary bootloader.elf bootloader.bin
    COMMENT "Converting bootloader ELF to BIN"
)
