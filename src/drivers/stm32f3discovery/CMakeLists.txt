# Add subdirectories
add_subdirectory(common)
add_subdirectory(io)
add_subdirectory(leds)
add_subdirectory(mcu)
add_subdirectory(timers)
add_subdirectory(usart)

# Create an umbrella "drivers" library that links the component libraries
add_library(stm32f3discovery INTERFACE)

target_link_libraries(stm32f3discovery
    INTERFACE
        drivers_common
        io
        leds
        mcu
        timers
        usart
)
