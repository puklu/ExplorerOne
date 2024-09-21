// src/main.cpp
#include "drivers/io/io.hpp"

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

void blinkyTestFunction(IO::ePin pin)
{
    // Enable GPIO clock
    IO::Enable(pin);

    // // Set pin as output
    IO::SetDirection(pin, IO::eDirection::IO_DIRECTION_OUTPUT);

    while (1)
    {
        IO::WriteOutputValue(pin, IO::eValue::IO_VALUE_HIGH);
        delay(100000);
        IO::WriteOutputValue(pin, IO::eValue::IO_VALUE_LOW);
        delay(100000);
    }
}

int main()
{
    blinkyTestFunction(IO::ePin::IO_TEST_LED_LD6);
}
