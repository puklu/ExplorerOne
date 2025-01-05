#include "common/assertHandler.hpp"
#include "common/registerArrays.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "GpioPin.hpp"


GpioPin::GpioPin(const GpioPinInitStruct &pin_init_struct)
    : PinBase(pin_init_struct.pin_name)
{
    SetMode(pin_init_struct.mode);
    SetOutputType(pin_init_struct.output_type);
    SetOutputSpeed(pin_init_struct.output_speed);
    SetResistor(pin_init_struct.pupd_resistor);
    mIsInitialized = true;
 }


void GpioPin::SetMode(IO::eMode mode)
{
    // TODO: add assert
    mMode = mode;

    switch (mode)
    {
        case IO::eMode::IO_MODE_INPUT:
            mpPort->MODER &= ~IO::aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~IO::aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_OUTPUT:
            mpPort->MODER &= ~IO::aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= IO::aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ALT_FUNCTION:
            mpPort->MODER |= IO::aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~IO::aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ANALOG:
            mpPort->MODER |= IO::aModeRegisterBits[2 * mPinNumber];
            mpPort->MODER |= IO::aModeRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

IO::eMode GpioPin::GetMode()
{
    return mMode;
}


void GpioPin::SetOutputType(IO::eOutputType outType)
{
    // TODO: add assert
    mOutputType                  = outType;

    switch (outType)
    {
        case IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL:
            mpPort->OTYPER &= ~IO::aOutputTypeRegisterBits[mPinNumber];
            break;
        case IO::eOutputType::IO_OUTPUT_TYPE_OPEN_DRAIN:
            mpPort->OTYPER |= IO::aOutputTypeRegisterBits[mPinNumber];
            break;
        default:
            break;
    }
}

void GpioPin::SetOutputSpeed(IO::eOutputSpeed outSpeed)
{
    // TODO: add assert
    mOutputSpeed                 = outSpeed;

    switch (outSpeed)
    {
        case IO::eOutputSpeed::IO_OUTPUT_LOW_SPEED:
            mpPort->MODER &= ~IO::aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~IO::aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_MEDIUM_SPEED:
            mpPort->MODER &= ~IO::aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= IO::aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_HIGH_SPEED:
            mpPort->MODER |= IO::aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= IO::aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        default:
            break;
    }
}

void GpioPin::SetResistor(IO::ePupdResistor updown)
{
    // TODO: add assert
    mPupdResistor                = updown;

    switch (updown)
    {
        case IO::ePupdResistor::IO_RESISTOR_NO_PUPD:
            mpPort->PUPDR &= ~IO::aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR &= ~IO::aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_UP:
            mpPort->PUPDR &= ~IO::aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR |= IO::aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_DOWN:
            mpPort->PUPDR |= IO::aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR &= ~IO::aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_RESERVED:
            mpPort->PUPDR |= IO::aPullupPulldownRegisterBits[2 * mPinNumber];
            mpPort->PUPDR |= IO::aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

IO::eValue GpioPin::ReadInputValue()
{
    // TODO: add assert
    mValueAtPin = IO::eValue(
        (mpPort->IDR & IO::aInputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

IO::eValue GpioPin::ReadOutputValue()
{
    // TODO: add assert
    mValueAtPin = IO::eValue(
        (mpPort->ODR & IO::aOutputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

void GpioPin::WriteOutputValue(IO::eValue value)
{
    // TODO: add assert
    mValueAtPin                  = value;

    switch (value)
    {
        case IO::IO_VALUE_LOW:
            mpPort->ODR &= ~IO::aOutputDataRegisterBits[mPinNumber];
            break;

        case IO::IO_VALUE_HIGH:
            mpPort->ODR |= IO::aOutputDataRegisterBits[mPinNumber];
            break;

        default:
            break;
    }
}
