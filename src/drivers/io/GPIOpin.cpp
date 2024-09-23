#include "GPIOpin.hpp"

#include "registerArrays.hpp"

namespace IO
{

GPIOpin::GPIOpin(ePin pin_name) : mPinName(pin_name)
{
    SetPortNumber();
    SetPinNumber();
    Enable();
    mIsInitialized = true;
}

void GPIOpin::SetPortNumber()
{
    mPortNumber = (mPinName & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

void GPIOpin::SetPinNumber()
{
    mPinNumber = mPinName & IO_PIN_MASK;
}

void GPIOpin::Enable() const
{
    // TODO: add assert
    RCC->AHBENR |= aPortEnableRegisters[mPortNumber];
}

void GPIOpin::SetMode(IO::eMode mode)
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];
    mMode                        = mode;

    switch (mode)
    {
        case IO::eMode::IO_MODE_INPUT:
            pPort->MODER &= ~aModeRegisterBits[2 * mPinNumber + 1];
            pPort->MODER &= ~aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_OUTPUT:
            pPort->MODER &= ~aModeRegisterBits[2 * mPinNumber + 1];
            pPort->MODER |= aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ALT_FUNCTION:
            pPort->MODER |= aModeRegisterBits[2 * mPinNumber + 1];
            pPort->MODER &= ~aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ANALOG:
            pPort->MODER |= aModeRegisterBits[2 * mPinNumber];
            pPort->MODER |= aModeRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

void GPIOpin::SetOutputType(eOutputType outType)
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];
    mOutputType                  = outType;

    switch (outType)
    {
        case IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL:
            pPort->OTYPER &= ~aOutputTypeRegisterBits[mPinNumber];
            break;
        case IO::eOutputType::IO_OUTPUT_TYPE_OPEN_DRAIN:
            pPort->OTYPER |= aOutputTypeRegisterBits[mPinNumber];
            break;
        default:
            break;
    }
}

void GPIOpin::SetOutputSpeed(eOutputSpeed outSpeed)
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];
    mOutputSpeed                 = outSpeed;

    switch (outSpeed)
    {
        case IO::eOutputSpeed::IO_OUTPUT_LOW_SPEED:
            pPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            pPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_MEDIUM_SPEED:
            pPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            pPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_HIGH_SPEED:
            pPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            pPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        default:
            break;
    }
}

void GPIOpin::SetResistor(IO::ePupdResistor updown)
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];
    mPupdResistor                = updown;

    switch (updown)
    {
        case IO::ePupdResistor::IO_RESISTOR_NO_PUPD:
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_UP:
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_DOWN:
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_RESERVED:
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber];
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

IO::eValue GPIOpin::ReadInputValue()
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];

    mValueAtPin = IO::eValue(
        (pPort->IDR & aInputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

IO::eValue GPIOpin::ReadOutputValue()
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];

    mValueAtPin = IO::eValue(
        (pPort->ODR & aOutputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

void GPIOpin::WriteOutputValue(IO::eValue value)
{
    // TODO: add assert
    volatile GPIO_TypeDef *pPort = aPorts[mPortNumber];
    mValueAtPin                  = value;

    switch (value)
    {
        case IO_VALUE_LOW:
            pPort->ODR &= ~aOutputDataRegisterBits[mPinNumber];
            break;

        case IO_VALUE_HIGH:
            pPort->ODR |= aOutputDataRegisterBits[mPinNumber];
            break;

        default:
            break;
    }
}

}  // namespace IO
