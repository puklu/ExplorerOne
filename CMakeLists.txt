cmake_minimum_required(VERSION 3.10)
project(ExplorerOne C CXX ASM)

# Specify the target chip
set(CHIP "STM32F303xC")

# option to use libopencm3 library
option(USE_LIBOPENCM3 "Build with libopencm3 support" OFF)

# Include complier and linker flags settings
include(cmake/flags.cmake)

# Include options to control logging level
include(cmake/logging.cmake)

# Include libopencm3
if(USE_LIBOPENCM3)
    include(cmake/libopencm3.cmake)
endif()

# Add subdirectory for src
add_subdirectory(src)

# Add subdirectory for external/printf
add_subdirectory(external/printf)

# Option to control whether tests are built
option(BUILD_TESTS "Built the test binaries" OFF)

# Add subdirectory for tests only if BUILD_TESTS is ON
if(BUILD_TESTS) 
    add_subdirectory(tests)
endif()

# Include formatting
include(cmake/formatting.cmake)

# Include cppcheck
include(cmake/cppcheck.cmake)

# Include generation of documentation
include(cmake/docs.cmake)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src" CACHE INTERNAL "Path to the src directory")


if(NOT DEFINED FIRMWARE_TARGET)
    message(FATAL_ERROR "FIRMWARE_TARGET not specified. Please set -DFIRMWARE_TARGET=<firmware target>")
endif()

# Select which firmware to flash
set(FIRMWARE_TARGET CACHE STRING "Firmware to flash (app or bootloader)")
message("FIRMWARE_TARGET is ${FIRMWARE_TARGET}")    

set(APP_ELF_FILE "${CMAKE_BINARY_DIR}/src/app/app.elf")
set(BOOTLOADER_ELF_FILE "${CMAKE_BINARY_DIR}/src/bootloader/bootloader.elf")

if(FIRMWARE_TARGET STREQUAL "bootloader")
    set(ELF_FILE ${BOOTLOADER_ELF_FILE})
else()
    set(ELF_FILE ${APP_ELF_FILE})
endif()        

# Define the path GDB script for flashing
set(FLASH_SCRIPT "${CMAKE_SOURCE_DIR}/tools/flash.gdb")
set(DEBUG_SCRIPT "${CMAKE_SOURCE_DIR}/tools/debug.gdb")

# Add a custom target to flash the firmware using GDB
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg &
    COMMAND sleep 2        # Ensure OpenOCD starts properly before connecting
    COMMAND gdb-multiarch --batch -x ${FLASH_SCRIPT} ${ELF_FILE}
    COMMAND pkill openocd  # Kill OpenOCD after GDB finishes
    DEPENDS ${ELF_FILE}
    COMMENT "Flashing firmware using GDB"
)

# Add a custom target to debug the firmware using GDB
add_custom_target(debug
    COMMAND openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg &
    COMMAND sleep 2        # Ensure OpenOCD starts properly before connecting
    COMMAND gdb-multiarch -x ${DEBUG_SCRIPT} ${ELF_FILE}
    COMMAND pkill openocd  # Kill OpenOCD after GDB finishes
    DEPENDS ${ELF_FILE}
    COMMENT "Flashing and starting a debugging session using GDB"
)

# Add a custom target for both bootloader and app
add_custom_target(all_firmware
    DEPENDS app.elf bootloader.elf
    COMMENT "Building both app and bootloader"
)

set(TEST_NAME "" CACHE STRING "Name of the test to flash")

set(TEST_ELF_FILE "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}.elf")

# Add a custom target to flash the tests using GDB
add_custom_target(flash_test
    COMMAND bash -c if [ "${TEST_NAME}" = "" ]; echo "Please specify TEST_NAME" ; exit 1; fi
    COMMAND openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg &
    COMMAND sleep 2        # Ensure OpenOCD starts properly before connecting
    COMMAND gdb-multiarch --batch -x ${FLASH_SCRIPT} ${TEST_ELF_FILE}
    COMMAND pkill openocd  # Kill OpenOCD after GDB finishes
    DEPENDS ${TEST_ELF_FILE}
    COMMENT "Flashing ${TEST_NAME} test and starting a debugging session using GDB"
)


add_dependencies(app.elf format)
add_dependencies(app.elf cppcheck)
add_dependencies(bootloader.elf format)
add_dependencies(bootloader.elf cppcheck)
