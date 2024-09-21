cmake_minimum_required(VERSION 3.10)
project(ExplorerOne C CXX ASM)

# Specify the target chip
set(CHIP "STM32F303xC")

# Include complier and linker flags settings
include(cmake/flags.cmake)

# Add subdirectory for drivers
add_subdirectory(src/drivers)

# Add subdirectory for src
add_subdirectory(src)

# Include formatting
include(cmake/formatting.cmake)

# Include cppcheck
include(cmake/cppcheck.cmake)

# Include generation of documentation
include(cmake/docs.cmake)

# Define the path to elf file and GDB script for flashing
set(ELF_FILE "${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.elf")
set(FLASH_SCRIPT "${CMAKE_SOURCE_DIR}/tools/flash.gdb")
set(DEBUG_SCRIPT "${CMAKE_SOURCE_DIR}/tools/debug.gdb")

# Add a custom target to flash the firmware using GDB
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg &
    COMMAND gdb-multiarch -x ${FLASH_SCRIPT} ${ELF_FILE}
    COMMAND sleep 2        # Ensure OpenOCD starts properly before connecting
    COMMAND pkill openocd  # Kill OpenOCD after GDB finishes
    DEPENDS ${ELF_FILE}
    COMMENT "Flashing firmware using GDB"
)

# Add a custom target to debug the firmware using GDB
add_custom_target(debug
    COMMAND openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg &
    COMMAND gdb-multiarch -x ${DEBUG_SCRIPT} ${ELF_FILE}
    COMMAND sleep 2        # Ensure OpenOCD starts properly before connecting
    COMMAND pkill openocd  # Kill OpenOCD after GDB finishes
    DEPENDS ${ELF_FILE}
    COMMENT "Flasing and starting a debugging session using GDB"
)

add_dependencies(${PROJECT_NAME}.elf clang-format)
add_dependencies(${PROJECT_NAME}.elf run_cppcheck)
