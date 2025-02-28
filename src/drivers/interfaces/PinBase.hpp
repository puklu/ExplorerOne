/**
 * @file PinBase.hpp
 * @brief Defines the base class for all pin types, providing essential pin functionality and data members.
 *
 * This file contains the declaration of the PinBase class and its supporting data structure.
 * The PinBase class acts as an abstract base class that provides common pin-related functionality, 
 * such as setting port and pin numbers, enabling the pin, and retrieving pin-specific information.
 */

#pragma once

#include <cstdint>
#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "IPin.hpp"


/**
 * @struct PinBaseInitStruct
 * @brief A structure for basic pin initialization.
 *
 * This structure holds the basic initialization information for a pin, including the pin name.
 */
struct PinBaseInitStruct
{
    IO::ePin pin_name;
};


/**
 * @class PinBase
 * @brief Abstract base class providing fundamental properties and operations for all pin types.
 *
 * The PinBase class serves as a foundation for specific pin types, such as GPIO or EXTI pins.
 * It provides core functionality like enabling the pin, setting port and pin numbers, and accessing
 * port and pin details. Derived classes should use this class to extend pin functionality.
 */
class PinBase  //: public IPin
{
public:
    /**
     * @brief Enables the pin by configuring the necessary registers.
     *
     * This function enables the clock for the pin’s associated port to ensure it can be used.
     */
    void Enable();

    /**
     * @brief Sets the port number based on the pin name.
     *
     * This function calculates and assigns the port number for the pin by interpreting the pin name.
     */
    void SetPortNumber();

    /**
     * @brief Sets the pin number based on the pin name.
     *
     * This function calculates and assigns the pin number within its port by interpreting the pin name.
     */
    void SetPinNumber();

    /**
     * @brief Retrieves the port number.
     * @return The port number assigned to this pin.
     */
    uint8_t GetPortNumber();

        /**
     * @brief Retrieves the port address.
     * @return The pointer to port assigned to this pin.
     */
    GPIO_TypeDef* GetPort();

    /**
     * @brief Retrieves the pin number within its port.
     * @return The pin number assigned to this pin.
     */
    uint8_t GetPinNumber();

    /**
     * @brief Destructor that safely cleans up the pin.
     *
     * The destructor removes the pin from any active pin tracking structure.
     */
    virtual ~PinBase();

    /**
     * @brief Deletes a pin object.
     *
     * This static function allows the deletion of a dynamically created pin object, ensuring safe
     * cleanup of the object.
     *
     * @param pin Pointer to the PinBase object to delete.
     */
    static void DeletePin(PinBase *pin);

protected:
    /**
     * @brief Protected constructor used by derived classes to initialize the pin.
     *
     * @param pin_name The name or identifier of the pin.
     */
    explicit PinBase(IO::ePin pin_name);
    
    friend class PinFactory;  // Allows PinFactory to access the protected constructor
    
    IO::ePin        mPinName;
    uint8_t         mPinNumber     = UINT8_MAX;
    uint8_t         mPortNumber    = UINT8_MAX;
    GPIO_TypeDef   *mpPort;
    RCC_TypeDef    *mpRCC          = RCC;
    bool            mIsInitialized = false;
};
