#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp" // TODO: Get rid of this. shouldnt need platfrom specific include?
#include "pinBank.hpp"
#include "PinBase.hpp"


PinBase::PinBase(IO::ePin pin_name): 
    mPinName(pin_name)
{
    SetPortNumber();
    SetPinNumber();
    Enable();
}

void PinBase::SetPortNumber()
{
    mPortNumber = (mPinName & IO_PORT_MASK) >> IO_PORT_OFFSET;
    ASSERT(mPortNumber < IO_PORT_COUNT);
    mpPort = IO::aPorts[mPortNumber];
}

void PinBase::SetPinNumber()
{
    mPinNumber = mPinName & IO_PIN_MASK;
    ASSERT(mPinNumber < IO_PIN_COUNT_PER_PORT);
}

uint8_t PinBase::GetPortNumber()
{
    ASSERT(mPortNumber != UINT8_MAX);
    return mPortNumber;
}

GPIO_TypeDef* PinBase::GetPort()
{
    return mpPort;
}

uint8_t PinBase::GetPinNumber()
{
    ASSERT(mPinNumber != UINT8_MAX);
    return mPinNumber;
}


void PinBase::Enable()
{
    // Enable the clock for the port
    mpRCC->AHBENR |= IO::aPortEnableRegisters[mPortNumber];
}

PinBase::~PinBase()
{
    activePins[mPortNumber][mPinNumber] = nullptr;
}


void PinBase::DeletePin(PinBase *pin)
{
    if(pin != nullptr)
    {
        delete pin;
    }
}
