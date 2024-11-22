# List of source files
set(MAIN_SOURCES 
    main.cpp
)

# Create the executable
add_executable(${PROJECT_NAME}.elf ${MAIN_SOURCES} ${START_UP_FILE})

# Add subdirectories
# add_subdirectory(app)
add_subdirectory(common)
add_subdirectory(drivers)
add_subdirectory(stm)
add_subdirectory(sys/stm32f3)

# Link the drivers library with the executable
target_link_libraries(${PROJECT_NAME}.elf
    PRIVATE
        # app
        common
        drivers
        printf
        stm
        sys_stm32f3
)

# Convert ELF to binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to BIN"
)

# Include directories
target_include_directories(${PROJECT_NAME}.elf 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/src
)
