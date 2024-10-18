#include "GPIOpin.hpp"

#include "common/registerArrays.hpp"
#include "common/assertHandler.hpp"
#include "pinBank.hpp"

namespace IO
{

GPIOpin* GPIOpin::CreatePin(const GpioPinInitStruct &pin_init_struct){
    
    // Get the port number and pin number
    uint8_t port_num = (pin_init_struct.pin_name & IO_PORT_MASK) >> IO_PORT_OFFSET;
    uint8_t pin_num = pin_init_struct.pin_name & IO_PIN_MASK;

    // Check if an instance of the pin already exists.
    // if it does, dont create an instance for the pin
    if(activePins[port_num][pin_num] != nullptr){
        return activePins[port_num][pin_num];
    }

    // otherwise create an instance and return it
    GPIOpin *pin = new GPIOpin(pin_init_struct);
    activePins[port_num][pin_num] = pin;

    return pin;
}


 GPIOpin::GPIOpin(const GpioPinInitStruct &pin_init_struct){
    mPinName = pin_init_struct.pin_name;
    Enable();
    SetMode(pin_init_struct.mode);
    SetOutputType(pin_init_struct.output_type);
    SetOutputSpeed(pin_init_struct.output_speed);
    SetResistor(pin_init_struct.pupd_resistor);
    mIsInitialized = true;
 }

void GPIOpin::Enable()
{
    SetPortNumber();
    SetPinNumber();

    // Enable the clock for the port
    mpRCC->AHBENR |= aPortEnableRegisters[mPortNumber];
}

void GPIOpin::SetPortNumber()
{
    mPortNumber = (mPinName & IO_PORT_MASK) >> IO_PORT_OFFSET;
    ASSERT(mPortNumber < IO_PORT_COUNT);
    mpPort = aPorts[mPortNumber];
}

uint8_t GPIOpin::GetPortNumber()
{
    ASSERT(mPortNumber != UINT8_MAX);
    return mPortNumber;
}

void GPIOpin::SetPinNumber()
{
    mPinNumber = mPinName & IO_PIN_MASK;
    ASSERT(mPinNumber < IO_PIN_COUNT_PER_PORT);
}

uint8_t GPIOpin::GetPinNumber()
{
    ASSERT(mPinNumber != UINT8_MAX);
    return mPinNumber;
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

void GPIOpin::SetAlternateFunction(eAlternateFunction alternate_function)
{
    ASSERT(mMode == IO::eMode::IO_MODE_ALT_FUNCTION);

    if(mPinNumber<8)
    {
        mpPort->AFR[0] &= ~(0xF << (mPinNumber * 4));
        mpPort->AFR[0] |= (static_cast<uint8_t>(alternate_function) << (mPinNumber * 4));
    }
    else
    {
        uint8_t pin_number = mPinNumber; // Making a copy to use here
        pin_number -= 8;

        mpPort->AFR[1] &= ~(0xF << (pin_number * 4));
        mpPort->AFR[1] |= (static_cast<uint8_t>(alternate_function) << (pin_number * 4));
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

/**
For the external interrupt lines, to generate the interrupt, the interrupt line should be
configured and enabled. This is done by programming the two trigger registers with the
desired edge detection and by enabling the interrupt request by writing a ‘1’ to the
corresponding bit in the interrupt mask register. When the selected edge occurs on the
external interrupt line, an interrupt request is generated. The pending bit corresponding to
the interrupt line is also set. This request is reset by writing a 1 in the pending register.
For the internal interrupt lines, the active edge is always the rising edge. The interrupt is
enabled by default in the interrupt mask register and there is no corresponding pending bit
in the pending register.
For the external lines, an interrupt/event request can also be generated by software by
writing a 1 in the software interrupt/event register.

To configure a line as interrupt source, use the following procedure:
- Configure the corresponding mask bit in the EXTI_IMR register.
- Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR)
- Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
EXTI so that an interrupt coming from one of the EXTI lines can be correctly
acknowledged.

Any of the external lines can be configured as software interrupt/event lines. The following is
the procedure to generate a software interrupt.
- Configure the corresponding mask bit (EXTI_IMR, EXTI_EMR)
- Set the required bit of the software interrupt register (EXTI_SWIER)
*/
void GPIOpin::EnableInterrupt(InterruptCallback cb)
{
    // Make sure that the pin exists in the pinBank before moving ahead
    ASSERT(activePins[mPortNumber][mPinNumber] != nullptr);

    ASSERT(cb != nullptr);
    mInterruptCallbackFunction = cb;

    // Enable system configuration
    mpRCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 
    // // Map the GPIO pin to EXTI line
    mpSystemConfigController->EXTICR[mPinNumber / 4] |= aSyscfgExtiRegisterBits[mPinNumber];
    mpSystemConfigController->EXTICR[mPinNumber / 4] &= (mPortNumber << (4* (mPinNumber % 4)));


    // Unmask the Interrupt mask register bit for the EXTI line. EXTI line number
    // corresponds to the pin number so this works out
    mpInterruptController->IMR |= (1<<mPinNumber);

    mIrqNumber = GetIRQn();

    EnableNVIC(); 
}


void GPIOpin::DisableInterrupt(){
    mpInterruptController->IMR &= ~(1<<mPinNumber);
    NVIC_DisableIRQ(mIrqNumber);
}

void GPIOpin::EnableNVIC(){
    NVIC_EnableIRQ(mIrqNumber);

    // Set priority
    NVIC_SetPriority(mIrqNumber, 2);

  }

bool GPIOpin::isInterruptPresent() const {
    return (mpInterruptController->PR & (1<<mPinNumber));
}  

void GPIOpin::ClearInterrupt(){
    if(isInterruptPresent())
    {
        mpInterruptController->PR |= (1<<mPinNumber);
    }
}  

IRQn_Type GPIOpin::GetIRQn() const{
    return aIrqType[mPinNumber]; 
}

InterruptCallback GPIOpin::GetInterruptCallback(){
    return mInterruptCallbackFunction;
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
        break;
    } 
}

}  // namespace IO
