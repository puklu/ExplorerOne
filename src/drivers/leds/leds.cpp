#include "leds.hpp"


void InterruptLed(){
    IO::GPIOpin interruptLedPin = IO::GPIOpin(IO::ePin::IO_TEST_LED_LD5_ORANGE);
    interruptLedPin.SetOutputType(IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL);
    interruptLedPin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
    interruptLedPin.SetMode(IO::eMode::IO_MODE_OUTPUT);

    uint8_t blinkCount = 0;

    while(blinkCount++ < 7){
    interruptLedPin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
    delay(350000);
    interruptLedPin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    delay(250000);
    }

}

void BlinkLed(IO::GPIOpin &pin)
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
