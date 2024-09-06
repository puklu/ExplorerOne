cmake_minimum_required(VERSION 3.10)
project(ExplorerOne C CXX ASM)

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
set(SOURCES src/main.cpp src/startup_stm32f303vctx.s)

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "drivers/CMSIS") 
set(STM32F3XX_DIR "drivers/CMSIS/Device/ST/STM32F3xx")

include_directories(${CMSIS_DIR}/Include)
include_directories(${STM32F3XX_DIR}/Include)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Convert ELF to binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to BIN"
)
