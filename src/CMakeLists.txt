# List of source files
set(MAIN main.cpp)
set(START_UP_FILE startup_stm32f303vctx.s)

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "${CMAKE_SOURCE_DIR}/src/drivers/stm/CMSIS")
set(STM32F3XX_DIR "${CMAKE_SOURCE_DIR}/src/drivers/stm/CMSIS/Device/ST/STM32F3xx")

# Add these files to a library
add_library(main ${MAIN} ${START_UP_FILE})

# Include directories
target_include_directories(main PUBLIC ${CMSIS_DIR}/Include ${STM32F3XX_DIR}/Include)
