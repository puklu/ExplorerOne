#pragma once

#include <cstdint>
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "stm32f303xc.h"

class IPin
{

public:
    // typedef void (*InterruptCallback)(void);

    virtual ~IPin(){}
   
    virtual void Enable() = 0;
    // virtual void SetMode(IO::eMode mode) = 0;
    virtual uint8_t GetPortNumber() = 0;
    virtual uint8_t GetPinNumber() = 0;
    virtual void SetPortNumber() = 0;
    virtual void SetPinNumber() = 0;
    // virtual void EnableInterrupt(InterruptCallback cb) = 0;
    // virtual void DisableInterrupt() = 0;
    // virtual void SelectInterruptTrigger(IO::eTriggerEdge) = 0;
    // virtual void ClearInterrupt() = 0;
    // virtual bool isInterruptPresent() const = 0;
    // virtual InterruptCallback GetInterruptCallback() = 0;
    virtual void DeletePin(IPin *pin) = 0;
    // virtual void EnableNVIC() = 0;
    // virtual IRQn_Type GetIRQn() const = 0;
protected:
    IPin();
    IPin(const IPin *other);
    IPin* operator=(const IPin &other);
};

// typedef struct
// {
//     ePin pin_name;
//     eMode mode;
//     eOutputType output_type = IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET;
//     eOutputSpeed output_speed = IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET;
//     ePupdResistor pupd_resistor = IO::ePupdResistor::IO_RESISTOR_NOT_SET;
// } GpioPinInitStruct;

//     void SetAlternateFunction(eAlternateFunction alternate_function);
