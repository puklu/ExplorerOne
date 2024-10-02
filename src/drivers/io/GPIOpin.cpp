#include "GPIOpin.hpp"

#include "registerArrays.hpp"
#include "assertHandler.hpp"

namespace IO
{

GPIOpin::GPIOpin(ePin pin_name) : mPinName(pin_name)
{

    Enable();
    mIsInitialized = true;
}

void GPIOpin::Enable()
{
    // TODO: add assert
    SetPortNumber();
    SetPinNumber();
    mpRCC->AHBENR |= aPortEnableRegisters[mPortNumber];
}

void GPIOpin::SetPortNumber()
{
    mPortNumber = (mPinName & IO_PORT_MASK) >> IO_PORT_OFFSET;
    mpPort = aPorts[mPortNumber];
}

void GPIOpin::SetPinNumber()
{
    mPinNumber = mPinName & IO_PIN_MASK;
}


void GPIOpin::SetMode(IO::eMode mode)
{
    // TODO: add assert
    mMode = mode;

    switch (mode)
    {
        case IO::eMode::IO_MODE_INPUT:
            mpPort->MODER &= ~aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_OUTPUT:
            mpPort->MODER &= ~aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ALT_FUNCTION:
            mpPort->MODER |= aModeRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~aModeRegisterBits[2 * mPinNumber];
            break;
        case IO::eMode::IO_MODE_ANALOG:
            mpPort->MODER |= aModeRegisterBits[2 * mPinNumber];
            mpPort->MODER |= aModeRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

void GPIOpin::SetOutputType(eOutputType outType)
{
    // TODO: add assert
    mOutputType                  = outType;

    switch (outType)
    {
        case IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL:
            mpPort->OTYPER &= ~aOutputTypeRegisterBits[mPinNumber];
            break;
        case IO::eOutputType::IO_OUTPUT_TYPE_OPEN_DRAIN:
            mpPort->OTYPER |= aOutputTypeRegisterBits[mPinNumber];
            break;
        default:
            break;
    }
}

void GPIOpin::SetOutputSpeed(eOutputSpeed outSpeed)
{
    // TODO: add assert
    mOutputSpeed                 = outSpeed;

    switch (outSpeed)
    {
        case IO::eOutputSpeed::IO_OUTPUT_LOW_SPEED:
            mpPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_MEDIUM_SPEED:
            mpPort->MODER &= ~aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        case IO::eOutputSpeed::IO_OUTPUT_HIGH_SPEED:
            mpPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber + 1];
            mpPort->MODER |= aOutputSpeedRegisterBits[2 * mPinNumber];
            break;
        default:
            break;
    }
}

void GPIOpin::SetResistor(IO::ePupdResistor updown)
{
    // TODO: add assert
    mPupdResistor                = updown;

    switch (updown)
    {
        case IO::ePupdResistor::IO_RESISTOR_NO_PUPD:
            mpPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_UP:
            mpPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_DOWN:
            mpPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            mpPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * mPinNumber];
            break;
        case IO::ePupdResistor::IO_RESISTOR_RESERVED:
            mpPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber];
            mpPort->PUPDR |= aPullupPulldownRegisterBits[2 * mPinNumber + 1];
            break;
        default:
            break;
    }
}

IO::eValue GPIOpin::ReadInputValue()
{
    // TODO: add assert
    mValueAtPin = IO::eValue(
        (mpPort->IDR & aInputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

IO::eValue GPIOpin::ReadOutputValue()
{
    // TODO: add assert
    mValueAtPin = IO::eValue(
        (mpPort->ODR & aOutputDataRegisterBits[mPinNumber]) >> mPinNumber);

    return mValueAtPin;
}

void GPIOpin::WriteOutputValue(IO::eValue value)
{
    // TODO: add assert
    mValueAtPin                  = value;

    switch (value)
    {
        case IO_VALUE_LOW:
            mpPort->ODR &= ~aOutputDataRegisterBits[mPinNumber];
            break;

        case IO_VALUE_HIGH:
            mpPort->ODR |= aOutputDataRegisterBits[mPinNumber];
            break;

        default:
            break;
    }
}

void GPIOpin::EnableInterrupt(){

    // TODO: add assert

    // Enable system configuration
    mpRCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 
    // Map the GPIO pin to EXTI line
    mpSystemConfigController->EXTICR[mPinNumber / 4] |= aSyscfgExtiRegisterBits[mPinNumber];
    mpSystemConfigController->EXTICR[mPinNumber / 4] &= (mPortNumber << (4* (mPinNumber % 4)));
 
}

void GPIOpin::SelectInterruptTrigger(IO::eTriggerEdge edge){
    // TODO: add assert
    switch (edge)
    {
    case IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE:
        mpInterruptController->RTSR |= (1 << mPinNumber);
        break;

    case IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_FALLING_EDGE:
        mpInterruptController->FTSR |= (1 << mPinNumber);
        break;

    case IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_BOTH_EDGE:
        mpInterruptController->RTSR |= (1 << mPinNumber);
        mpInterruptController->FTSR |= (1 << mPinNumber);
        break;      
    default:
        ASSERT(0);
    }
    
}

}  // namespace IO
