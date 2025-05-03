#include "common/defines.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "leds.hpp"


void InterruptLed(){
    
    GpioPinInitStruct interruptLedPinInit = {};
    interruptLedPinInit.pin_name      = IO::ePin::IO_TEST_LED_LD5_ORANGE;
    interruptLedPinInit.mode          = IO::eMode::IO_MODE_OUTPUT;
    interruptLedPinInit.output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    interruptLedPinInit.pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> interruptLedPin = PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, interruptLedPinInit);
    auto gpio_pin = std::dynamic_pointer_cast<GpioPin>(interruptLedPin);

    gpio_pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
    DELAY(1000_ms);
    gpio_pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
 
}

void BlinkLed(GpioPin *pin)
{
    while (1)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        DELAY(1000_ms);
        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        DELAY(1000_ms);
    }
}
