# List of source files
set(MAIN_SOURCES main.cpp)
set(START_UP_FILE startup_stm32f303vctx.s)

set(DRIVERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/drivers")
set(DRIVERS_DIR_IO "${CMAKE_CURRENT_SOURCE_DIR}/drivers/io")

# Create the executable
add_executable(${PROJECT_NAME}.elf ${MAIN_SOURCES} ${START_UP_FILE})

# Link the drivers library with the executable
target_link_libraries(${PROJECT_NAME}.elf PRIVATE drivers)

# Convert ELF to binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to BIN"
)

# Include directories
target_include_directories(${PROJECT_NAME}.elf 
                           PUBLIC 
                           ${DRIVERS_DIR})
