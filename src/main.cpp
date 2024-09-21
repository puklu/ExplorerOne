// src/main.cpp
#include "drivers/io/io.hpp"
#include "stm32f303xc.h"

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main()
{
    // Enable GPIO clock
    IO::Enable(IO::ePin::IO_TEST_LED_LD3);

    // // Set pin as output
    IO::SetDirection(IO::ePin::IO_TEST_LED_LD3,
                     IO::eDirection::IO_DIRECTION_OUTPUT);

    while (1)
    {
        IO::WriteOutputValue(IO::ePin::IO_TEST_LED_LD3,
                             IO::eValue::IO_VALUE_HIGH);
        delay(100000);
        IO::WriteOutputValue(IO::ePin::IO_TEST_LED_LD3,
                             IO::eValue::IO_VALUE_LOW);
        delay(100000);
    }
}
