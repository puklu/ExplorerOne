# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS")
set(STM32F3XX_DIR "${CMSIS_DIR}/Device/ST/STM32F3xx")

# Add these files to a library
add_library(stm INTERFACE)

# Include directories
target_include_directories(stm 
    INTERFACE
        ${CMSIS_DIR}/Include      # To be able just use the stm header without typing complete path
        ${STM32F3XX_DIR}/Include
)
