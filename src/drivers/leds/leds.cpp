#include "leds.hpp"


void InterruptLed(){

    IO::GpioPinInitStruct interruptLedPinInit = {
        .pin_name      = IO::ePin::IO_TEST_LED_LD5_ORANGE,
        .mode          = IO::eMode::IO_MODE_OUTPUT,
        .output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL,
        .pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    IO::GPIOpin *interruptLedPin = IO::GPIOpin::CreatePin(interruptLedPinInit);


    uint8_t blinkCount = 0;

    while(blinkCount++ < 7){
    interruptLedPin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
    delay(350000);
    interruptLedPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    delay(250000);
    }

}

void BlinkLed(IO::GPIOpin &pin)
{
    while (1)
    {
        pin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(250000);
        pin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(250000);
    }
}
