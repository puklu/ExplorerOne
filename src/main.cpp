// src/main.cpp
extern "C"
{
    int main();
}

#include "stm32f303xc.h"

extern "C" void SystemInit()
{
    // System initialization code goes here, or leave it empty
}

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main()
{
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

    // Set PC9 as output
    GPIOE->MODER |= GPIO_MODER_MODER8_0;

    while (1)
    {
        // Toggle LED
        GPIOE->ODR ^= GPIO_ODR_8;
        delay(100000);
    }
}
