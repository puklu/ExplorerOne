/**
 * @file PinFactory.hpp
 * @brief Provides a factory interface for creating and managing various types of pin objects.
 *
 * This file declares the PinFactory class, which is responsible for instantiating different types
 * of pins based on their type and initialization structure. The factory ensures that each pin type
 * is created with the appropriate configuration and initialized according to its specific requirements.
 */

#pragma once

#include <memory>
#include "PinBase.hpp"
#include "common/PinDefinitions.hpp"

/**
 * @class PinFactory
 * @brief Factory class for creating instances of different types of pins.
 *
 * The PinFactory class centralizes the creation of pin instances based on their specific type.
 * It provides a single interface for creating pins, where each pin type is constructed using
 * a provided initialization structure.
 */
class PinFactory
{
    public:
        static std::shared_ptr<PinBase> CreatePin(IO::ePinType pin_type, const PinBaseInitStruct &pin_init_struct);
};
