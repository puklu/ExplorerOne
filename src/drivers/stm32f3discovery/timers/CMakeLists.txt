# List of source files
set(TIMERS_SOURCES
    BaseTimer.cpp
    BasicTimer.cpp
    GeneralPurposeTimer.cpp
    GeneralPurposeTimerConfig.cpp
    ServiceISRs.cpp
    TimerChannel.cpp
)

# Add these files to a library
add_library(timers ${TIMERS_SOURCES})

# Other libraries needed
target_link_libraries(timers 
    PRIVATE 
        common
        interfaces
        leds
    PUBLIC
        stm    
)

# Include directories
target_include_directories(timers
    PUBLIC                        # make these include paths available others that link to this library
        ${CMAKE_SOURCE_DIR}/src
)
