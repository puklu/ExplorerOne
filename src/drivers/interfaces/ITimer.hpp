/**
 * @file ITimer.hpp
 * 
 * @brief Interface for timer operations.
 */

#pragma once

#include <memory>
#include <vector>
#include "common/PinDefinitions.hpp"
#include "common/time.hpp"
#include "ITimerChannel.hpp"

/**
 * @class ITimer
 * 
 * @brief An interface for timer operations.
 * 
 * This interface defines the essential functionalities required for a timer, 
 * such as starting, stopping, resetting, and configuring the timer. It provides 
 * a common structure that all timer classes must adhere to, ensuring consistent 
 * operations across different timer implementations.
 * 
 * The interface allows flexibility for various timer configurations, including 
 * setting the period, count, enabling/disabling interrupts, and handling timer 
 * states like start, stop, and reset.
 * 
 * Classes implementing this interface should handle interactions with hardware 
 * timers and manage low-level timer functionalities based on the provided 
 * configurations.
 * 
 * @note This is an abstract interface and must be implemented by specific timer 
 *       classes to work with different hardware timer peripherals.
 */
class ITimer
{
public:
    virtual eGeneralStatus Start() = 0;
    virtual eGeneralStatus Stop() = 0;
    virtual eGeneralStatus Reset() = 0;
    virtual float GetSysClockTicksElapsed() const = 0;
    virtual Microseconds GetTimeElapsedInMicroseconds() const = 0;
    virtual Milliseconds GetTimeElapsedInMilliseconds() const = 0;
    virtual eGeneralStatus SetPeriodAndCount(Milliseconds period, uint32_t count) = 0;
    virtual eGeneralStatus EnableInterrupt() = 0;
    virtual eGeneralStatus DisableInterrupt() = 0;
    virtual std::vector<std::shared_ptr<ITimerChannel>> GetChannels() = 0;
    virtual eGeneralStatus SetPeriodAndDutyCycle(Milliseconds period_in_ms, uint32_t duty_cycle, uint8_t channel_index) = 0;
    virtual bool GetIsTimerRunning() const = 0;
    virtual ~ITimer() = default;
};
