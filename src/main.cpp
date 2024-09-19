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
    IO::enable(IO::pin::IO_TEST_LED);

    // // Set PC9 as output
    // GPIOE->MODER |= GPIO_MODER_MODER8_0;
    IO::set_direction(IO::pin::IO_TEST_LED, IO::direction::IO_DIRECTION_OUTPUT);

    // while (1)
    // {
    //     // Toggle LED
    //     GPIOE->ODR ^= GPIO_ODR_8;
    //     delay(100000);
    // }
}
