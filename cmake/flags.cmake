# Set the cross-compiler
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")

# Set compiler flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(COMMON_FLAGS
    -g3 
    -O0 
    -Wall 
    -Wextra 
    -Wshadow 
    -fdata-sections 
    -ffunction-sections
    )

# libopencm3 causes many error because things gets redefined since they also exist
# in CMSIS, so turn off Werror when libopencm3 is used
if(NOT USE_LIBOPENCM3)
    list(APPEND COMMON_FLAGS -Werror)
endif()

# convert the flags into a string for the following lines
string(JOIN " " COMMON_FLAGS_STR ${COMMON_FLAGS})

set(CMAKE_C_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS_STR}")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS_STR} -fno-exceptions")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS} ${COMMON_FLAGS_STR}")

# Linker flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/sys/stm32f3/STM32F303VCTX_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -T ${LINKER_SCRIPT} -Wl,--gc-sections")

# Startup file
set(START_UP_FILE "${CMAKE_SOURCE_DIR}/src/sys/stm32f3/startup_stm32f303vctx.s")
