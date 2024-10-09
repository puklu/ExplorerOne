/**
 * @file pinBank.hpp
 * @brief Manages the active GPIO pin instances.
 *
 * This header file defines an array of pointers to GPIOpin instances, 
 * allowing for easy access and management of active GPIO pins within 
 * the application. The `activePins` array is indexed by port and 
 * pin numbers, providing a structured way to reference the GPIO pins 
 * available in the system.
 * 
 * @note The `activePins` array is initialized to nullptr to indicate 
 * that no pins are active by default. This array should be used in 
 * conjunction with the GPIOpin class to manage GPIO functionality 
 * across the application.
 */

#pragma once

#include "common/defines.hpp"

// Array to hold active GPIOpin instances for each port and pin combination.
// Each entry can be nullptr if the corresponding pin is not currently active.
inline IO::GPIOpin *activePins[IO_PORT_COUNT][IO_PIN_COUNT_PER_PORT] = {nullptr};
