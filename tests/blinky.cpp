#include "blinky.hpp"

void blinkyTestFunction(IO::GPIOpin *pin)
{
    while (1)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(250000);
        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(250000);
    }
}

int main()
{
    IO::GpioPinInitStruct pinInit = {
        .pin_name      = IO::ePin::IO_TEST_LED_LD4_BLUE,
        .mode          = IO::eMode::IO_MODE_OUTPUT,
        .output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL,
        .pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    IO::GPIOpin *pin = IO::GPIOpin::CreatePin(pinInit);

    blinkyTestFunction(pin);
}