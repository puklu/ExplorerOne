cmake_minimum_required(VERSION 3.10)
project(ExplorerOne C CXX ASM)

find_program(CPPCHECK_EXECUTABLE NAMES cppcheck) 
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format) 

# Specify the target chip
set(CHIP "STM32F303xC")

# Set the cross-compiler
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")

# Set compiler flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb")
set(COMMON_FLAGS "-g3 -O0 -Wall -Werror -Wextra -Wshadow -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS}")

# Linker flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F303VCTX_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -T ${LINKER_SCRIPT} -Wl,--gc-sections")

# Specify source files
set(SOURCES src/main.cpp)
set(START_UP_FILE src/startup_stm32f303vctx.s)

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "src/drivers/stm/CMSIS")
set(STM32F3XX_DIR "src/drivers/stm/CMSIS/Device/ST/STM32F3xx")

include_directories(${CMSIS_DIR}/Include)
include_directories(${STM32F3XX_DIR}/Include)

# Enable compile_commands.json for better cppcheck analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${START_UP_FILE})

# Convert ELF to binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to BIN"
)

# Function to run clang-format
add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format..."
)

# Add a target to check code formatting (no -i flag)
add_custom_target(
    clang-format-check
    COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run -Werror ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Checking code format..."
)

#add_dependencies(${PROJECT_NAME}.elf clang-format)

if(CPPCHECK_EXECUTABLE)
    message(STATUS "Running statis analysis! Cppcheck found at: ${CPPCHECK_EXECUTABLE}")
    add_custom_target(run_cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE}
        --enable=all                            # enable all checks
        --inconclusive
        --quiet
        --suppress=missingInclude
        ${CMAKE_SOURCE_DIR}/src/                # direcotry to include for check
        -i${CMAKE_SOURCE_DIR}/src/drivers/stm   # directories to ignore for check
        # --project=${CMAKE_BINARY_DIR}/compile_commands.json # Use compile_commands.json for flags
        -D__GNUC__                              # Define GNU C compiler
        --verbose
    )

    # Run cppcheck before building
    add_dependencies(${PROJECT_NAME}.elf run_cppcheck)
else()
    message(WARNING "Cppcheck not found. Skipping static analysis.")    
endif()  
