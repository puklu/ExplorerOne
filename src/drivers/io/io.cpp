#include "io.hpp"

#include "registerArrays.hpp"

void SystemInit()
{
    // TODO: Move to MCU_init() or something
    // System initialization code goes here, or leave it empty
}

namespace IO
{
uint8_t GetPortNumber(IO::ePin pin_name)
{
    return (pin_name & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

uint8_t GetPinNumber(IO::ePin pin_name)
{
    return pin_name & IO_PIN_MASK;
}

void Enable(IO::ePin pin_name)
{
    uint8_t port_num = GetPortNumber(pin_name);
    RCC->AHBENR |= aPortEnableRegisters[port_num];
}

void SetDirection(IO::ePin pin_name, IO::eDirection dir)
{
    uint8_t                port_num = GetPortNumber(pin_name);
    uint8_t                pin_num  = GetPinNumber(pin_name);
    volatile GPIO_TypeDef *pPort    = aPorts[port_num];

    switch (dir)
    {
        case IO::eDirection::IO_DIRECTION_INPUT:
            pPort->MODER &= ~aModeRegisterBits[2 * pin_num + 1];
            pPort->MODER &= ~aModeRegisterBits[2 * pin_num];
            break;
        case IO::eDirection::IO_DIRECTION_OUTPUT:
            pPort->MODER &= ~aModeRegisterBits[2 * pin_num + 1];
            pPort->MODER |= aModeRegisterBits[2 * pin_num];
            break;
        case IO::eDirection::IO_DIRECTION_ALT_FUNCTION_MODE:
            pPort->MODER |= aModeRegisterBits[2 * pin_num + 1];
            pPort->MODER &= ~aModeRegisterBits[2 * pin_num];
            break;
        case IO::eDirection::IO_DIRECTION_ANALOG_MODE:
            pPort->MODER |= aModeRegisterBits[2 * pin_num];
            pPort->MODER |= aModeRegisterBits[2 * pin_num + 1];
            break;
        default:
            break;
    }
}

void SetResistor(IO::ePin pin_name, IO::ePupdResistor updown)
{
    uint8_t                port_num = GetPortNumber(pin_name);
    uint8_t                pin_num  = GetPinNumber(pin_name);
    volatile GPIO_TypeDef *pPort    = aPorts[port_num];

    switch (updown)
    {
        case IO::ePupdResistor::IO_RESISTOR_NO_PUPD:
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * pin_num + 1];
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * pin_num];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_UP:
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * pin_num + 1];
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * pin_num];
            break;
        case IO::ePupdResistor::IO_RESISTOR_PULL_DOWN:
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * pin_num + 1];
            pPort->PUPDR &= ~aPullupPulldownRegisterBits[2 * pin_num];
            break;
        case IO::ePupdResistor::IO_RESISTOR_RESERVED:
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * pin_num];
            pPort->PUPDR |= aPullupPulldownRegisterBits[2 * pin_num + 1];
            break;
        default:
            break;
    }
}

IO::eValue ReadInputValue(IO::ePin pin_name)
{
    uint8_t                port_num = GetPortNumber(pin_name);
    uint8_t                pin_num  = GetPinNumber(pin_name);
    volatile GPIO_TypeDef *pPort    = aPorts[port_num];

    IO::eValue value =
        IO::eValue((pPort->IDR & aInputDataRegisterBits[pin_num]) >> pin_num);

    return value;
}

IO::eValue ReadOutputValue(IO::ePin pin_name)
{
    uint8_t                port_num = GetPortNumber(pin_name);
    uint8_t                pin_num  = GetPinNumber(pin_name);
    volatile GPIO_TypeDef *pPort    = aPorts[port_num];

    IO::eValue value =
        IO::eValue((pPort->ODR & aOutputDataRegisterBits[pin_num]) >> pin_num);

    return value;
}

void WriteOutputValue(IO::ePin pin_name, IO::eValue value)
{
    uint8_t                port_num = GetPortNumber(pin_name);
    uint8_t                pin_num  = GetPinNumber(pin_name);
    volatile GPIO_TypeDef *pPort    = aPorts[port_num];

    switch (value)
    {
        case IO_VALUE_LOW:
            pPort->ODR &= ~aOutputDataRegisterBits[pin_num];
            break;

        case IO_VALUE_HIGH:
            pPort->ODR |= aOutputDataRegisterBits[pin_num];
            break;

        default:
            break;
    }
}

}  // namespace IO