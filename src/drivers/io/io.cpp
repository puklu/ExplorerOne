#include "io.hpp"

#include "stm32f303xc.h"

#define IO_PIN_MASK (0x0F)
#define IO_PORT_OFFSET (4u)
#define IO_PORT_MASK (0x70)
#define IO_PORT_COUNT (6u)
#define IO_PIN_COUNT_PER_COUNT (16u)
#define IO_BITS_PER_REGISTER (32u)

void SystemInit()
{
    // TODO: Move to MCU_init() or something
    // System initialization code goes here, or leave it empty
}

namespace IO
{
uint8_t get_port_number(IO::pin pin_name)
{
    return (pin_name & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

uint8_t get_pin_number(IO::pin pin_name)
{
    return pin_name & IO_PIN_MASK;
}

static volatile GPIO_TypeDef *const pointer_to_port[IO_PORT_COUNT] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
};

static volatile int const port_enable_register[IO_PORT_COUNT] = {
    RCC_AHBENR_GPIOAEN, RCC_AHBENR_GPIOBEN, RCC_AHBENR_GPIOCEN,
    RCC_AHBENR_GPIODEN, RCC_AHBENR_GPIOEEN, RCC_AHBENR_GPIOFEN};

static volatile long unsigned int const
    mode_register_bits[IO_BITS_PER_REGISTER] = {
        GPIO_MODER_MODER0_0,  GPIO_MODER_MODER0_1,  GPIO_MODER_MODER1_0,
        GPIO_MODER_MODER1_1,  GPIO_MODER_MODER2_0,  GPIO_MODER_MODER2_1,
        GPIO_MODER_MODER3_0,  GPIO_MODER_MODER3_1,  GPIO_MODER_MODER4_0,
        GPIO_MODER_MODER4_1,  GPIO_MODER_MODER5_0,  GPIO_MODER_MODER5_1,
        GPIO_MODER_MODER6_0,  GPIO_MODER_MODER6_1,  GPIO_MODER_MODER7_0,
        GPIO_MODER_MODER7_1,  GPIO_MODER_MODER8_0,  GPIO_MODER_MODER8_1,
        GPIO_MODER_MODER9_0,  GPIO_MODER_MODER9_1,  GPIO_MODER_MODER10_0,
        GPIO_MODER_MODER10_1, GPIO_MODER_MODER11_0, GPIO_MODER_MODER11_1,
        GPIO_MODER_MODER12_0, GPIO_MODER_MODER12_1, GPIO_MODER_MODER13_0,
        GPIO_MODER_MODER13_1, GPIO_MODER_MODER14_0, GPIO_MODER_MODER14_1,
        GPIO_MODER_MODER15_0, GPIO_MODER_MODER15_1,
};

void enable(IO::pin pin_name)
{
    uint8_t port_num = get_port_number(pin_name);
    RCC->AHBENR |= port_enable_register[port_num];
}

void set_direction(IO::pin pin_name, IO::direction dir)
{
    // TODO : Implement
    uint8_t port_num = get_port_number(pin_name);
    uint8_t pin_num = get_pin_number(pin_name);
    volatile GPIO_TypeDef *pPort = pointer_to_port[port_num];

    switch (dir)
    {
        case IO::direction::IO_DIRECTION_INPUT:
            pPort->MODER &= ~mode_register_bits[2 * pin_num + 1];
            pPort->MODER &= ~mode_register_bits[2 * pin_num];
            break;
        case IO::direction::IO_DIRECTION_OUTPUT:
            pPort->MODER &= ~mode_register_bits[2 * pin_num + 1];
            pPort->MODER |= mode_register_bits[2 * pin_num];
            break;
        case IO::direction::IO_DIRECTION_ALT_FUNCTION_MODE:
            pPort->MODER |= mode_register_bits[2 * pin_num + 1];
            pPort->MODER &= ~mode_register_bits[2 * pin_num];
            break;
        case IO::direction::IO_DIRECTION_ANALOG_MODE:
            pPort->MODER |= mode_register_bits[2 * pin_num];
            pPort->MODER |= mode_register_bits[2 * pin_num + 1];
            break;
        default:
            break;
    }
}

void set_resistor(IO::pin pin_name, IO::pupd_resistor resistor)
{
    // TODO : Implement
    (void)pin_name;
    (void)resistor;
}

}  // namespace IO