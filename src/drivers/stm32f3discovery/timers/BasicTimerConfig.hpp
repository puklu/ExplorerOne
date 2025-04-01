/**
 * @file BasicTimerConfig.hpp
 * 
 * @brief Header file containing configurations for basic timers.
 * The configurations is the same as BaseTimerConfig so it inherits all of them from the
 * base class.
 * 
 * The configurations stored in these classes are used to setup timers.
 * 
 */

 #include "BaseTimerConfig.hpp"

 #pragma once
  
/**
 * @brief Class representing the initialization configuration for a basic timer.
 * 
 * This class encapsulates the necessary configuration parameters for initializing
 * a basic timer, including prescaler value, auto-reload register value, interrupt
 * callback, counter mode, preload settings, update sources, and more.
 */
class BasicTimerConfig : public BaseTimerConfig
{
public:
 
   BasicTimerConfig() = default;

};
