#include "interrupt.hpp"

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/io/ExtiPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/mcu/mcuInit.hpp"

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_UNUSED_D0;
    pinInit.mode              = IO::eMode::IO_MODE_INPUT;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *exti_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_EXTI, pinInit);
    auto pin = static_cast<ExtiPin *>(exti_pin);

    pin->EnableInterrupt(InterruptLed);
    pin->SelectInterruptTrigger(
        IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE);

    while (1)
    {
    }
}
