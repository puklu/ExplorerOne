// src/main.cpp
#include <cstdint>  // for uint8_t

#include "drivers/io/GPIOpin.hpp"

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

void blinkyTestFunction(IO::GPIOpin &pin)
{
    pin.SetOutputType(IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL);
    pin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
    pin.SetMode(IO::eMode::IO_MODE_OUTPUT);

    while (1)
    {
        pin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(100000);
        pin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(100000);
    }
}

int main()
{
    IO::GPIOpin pin = IO::GPIOpin(IO::ePin::IO_TEST_LED_LD10);
    blinkyTestFunction(pin);
}
