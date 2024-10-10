#include "blinky.hpp"

void blinkyTestFunction(IO::GPIOpin &pin)
{
    while (1)
    {
        pin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(250000);
        pin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(250000);
    }
}
