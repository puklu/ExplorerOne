// src/main.cpp
#include "drivers/io/io.hpp"

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main()
{
    // Enable GPIOC clock
    // RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    IO::Enable(IO::ePin::IO_TEST_LED);

    // // Set PC9 as output
    // GPIOE->MODER |= GPIO_MODER_MODER8_0;
    IO::SetDirection(IO::ePin::IO_TEST_LED,
                     IO::eDirection::IO_DIRECTION_OUTPUT);

    // while (1)
    // {
    //     // Toggle LED
    //     GPIOE->ODR ^= GPIO_ODR_8;
    //     delay(100000);
    // }
}
