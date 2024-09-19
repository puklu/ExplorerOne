# List of source files
set(DRIVERS_SOURCES
    io/io.cpp
)

# Add these files to a library
add_library(drivers ${DRIVERS_SOURCES})

# Set the path to the CMSIS and STM32 headers
set(CMSIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm/CMSIS")
set(STM32F3XX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm/CMSIS/Device/ST/STM32F3xx")

# Include directories
target_include_directories(drivers PUBLIC 
                           ${CMAKE_SOURCE_DIR}/src/drivers
                           ${CMSIS_DIR}/Include 
                           ${STM32F3XX_DIR}/Include
)
