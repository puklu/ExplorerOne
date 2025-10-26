if(NOT DEFINED USE_LIBOPENCM3)
    set(USE_LIBOPENCM3 OFF CACHE BOOL "Use libopencm3")
endif()

# List of source files
set(MAIN_SOURCES 
    main.cpp
)

# Add subdirectories
add_subdirectory(app)
add_subdirectory(bootloader)
add_subdirectory(common)
add_subdirectory(drivers)
add_subdirectory(sys/stm32f3)

# App firmware target
# TODO: Ultimately there will not be a main.cpp here at this level,
# so this should change later
add_executable(app.elf ${MAIN_SOURCES} ${START_UP_FILE})

# Link the drivers library with the app executable
target_link_libraries(app.elf
    PRIVATE
        app
        common
        drivers
        printf
        sys_stm32f3
)

#  Apply app-specific linker script
target_link_options(app.elf
    PRIVATE
        "-T${CMAKE_SOURCE_DIR}/src/app/app.ld"
        "-Wl,--gc-sections"
)

# Tell the linker to generate a map file for app
set_target_properties(app.elf PROPERTIES
LINK_FLAGS "-Wl,-Map=app.map"
COMMENT "Generating Map file for app"
)

# Convert ELF to binary for app
add_custom_command(TARGET app.elf POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O binary app.elf app.bin
COMMENT "Converting app ELF to BIN"
)

if(USE_LIBOPENCM3)
    target_link_libraries(app.elf PRIVATE libopencm3)
    target_compile_definitions(app.elf PRIVATE USE_LIBOPENCM3)
endif()
