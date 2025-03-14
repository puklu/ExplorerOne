/**
 * @file pinBank.hpp
 * @brief Manages the active GPIO pin instances.
 *
 * This header file defines an array of pointers to GpioPin instances, 
 * allowing for easy access and management of active GPIO pins within 
 * the application. The `activePins` array is indexed by port and 
 * pin numbers, providing a structured way to reference the GPIO pins 
 * available in the system.
 * 
 * @note The `activePins` array is initialized to nullptr to indicate 
 * that no pins are active by default. This array should be used in 
 * conjunction with the GpioPin class to manage GPIO functionality 
 * across the application.
 */

#pragma once

#include <array>
#include "common/defines.hpp"
#include "drivers/interfaces/ITimer.hpp"
#include "PinBase.hpp"

// forward declaration for UsartPin class
class UsartPin;

// Array to hold active GpioPin instances for each port and pin combination.
// Each entry can be nullptr if the corresponding pin is not currently active.
inline std::array<std::array<std::shared_ptr<PinBase>, IO_PIN_COUNT_PER_PORT>, IO_PORT_COUNT> activePins{};

// Array to hold active ExtiPin instances for each port and pin combination.
// Each entry can be nullptr if the corresponding pin is not currently active.
inline std::array<std::array<std::shared_ptr<PinBase>, IO_PIN_COUNT_PER_PORT>, IO_PORT_COUNT> activeExtiPins{};

// Array to hold active UsartPin instances for each port and pin combination.
// Each entry can be nullptr if the corresponding pin is not currently active.
inline std::array<std::array<std::shared_ptr<PinBase>, IO_PIN_COUNT_PER_PORT>, IO_PORT_COUNT> activeUsartPins{};

// Holds the pin being used for printing on terminal
inline std::shared_ptr<UsartPin> activePrintUsartPin = nullptr;

// Array to hold the active basic timers
inline ITimer *basicTimers[NUMBER_OF_BASIC_TIMERS] = {nullptr, nullptr};

// Array to hold the active general purpose timers
inline ITimer *generalPurposeTimers[NUMBER_OF_GENERAL_PURPOSE_TIMERS] = {nullptr, nullptr, nullptr};
