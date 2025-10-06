#include "common/defines.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "leds.hpp"


Led::Led(std::shared_ptr<GpioPin> pin)
    :mPin(std::move(pin)), mBlinkPending(false)
{
}

void Led::RequestBlink()
{
    mBlinkPending = true;
}

void Led::Process()
{
    if(mBlinkPending){
        mBlinkPending = false;
        mPin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        DELAY(100_ms);
        mPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    }
}

void Led::BlinkLedForever()
{
    while (1)
    {
        mPin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        DELAY(1000_ms);
        mPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        DELAY(1000_ms);
    }
}
