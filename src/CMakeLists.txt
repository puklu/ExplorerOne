if(NOT DEFINED USE_LIBOPENCM3)
    set(USE_LIBOPENCM3 OFF CACHE BOOL "Use libopencm3")
endif()


# Add subdirectories
add_subdirectory(app)
add_subdirectory(bootloader)
add_subdirectory(common)
add_subdirectory(drivers)
add_subdirectory(sys/stm32f3)
