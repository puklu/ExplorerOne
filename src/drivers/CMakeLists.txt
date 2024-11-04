# Add subdirectories
add_subdirectory(io)
add_subdirectory(interfaces)
add_subdirectory(leds)
add_subdirectory(mcu)
add_subdirectory(usart)

# Create an umbrella "drivers" library that links the component libraries
add_library(drivers INTERFACE)

target_link_libraries(drivers
    INTERFACE
        common
        io
        interfaces
        leds
        mcu
        usart
)
