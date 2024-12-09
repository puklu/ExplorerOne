#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "leds.hpp"


void InterruptLed(){
    
    GpioPinInitStruct interruptLedPinInit = {};
    interruptLedPinInit.pin_name      = IO::ePin::IO_TEST_LED_LD5_ORANGE;
    interruptLedPinInit.mode          = IO::eMode::IO_MODE_OUTPUT;
    interruptLedPinInit.output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    interruptLedPinInit.pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *interruptLedPin = PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, interruptLedPinInit);
    auto gpio_pin = static_cast<GpioPin*>(interruptLedPin);

    gpio_pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
    delay(20000);
    gpio_pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
 
}

void BlinkLed(GpioPin &pin)
{
    while (1)
    {
        pin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(250000);
        pin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(250000);
    }
}
