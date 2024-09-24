# List of source files
set(DRIVERS_SOURCES
    io/GPIOpin.cpp
    mcu/mcuInit.cpp
)

# Add these files to a library
add_library(drivers ${DRIVERS_SOURCES})

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm/CMSIS")
set(STM32F3XX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm/CMSIS/Device/ST/STM32F3xx")

# Include directories
target_include_directories(drivers PUBLIC 
                           ${CMSIS_DIR}/Include      # To be able just use the stm header without typing complete path
                           ${STM32F3XX_DIR}/Include
                           ${SRC_DIR}
)
