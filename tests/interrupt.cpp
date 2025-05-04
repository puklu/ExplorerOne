#include "common/Delay.hpp"
#include "common/assertHandler.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/io/ExtiPin.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_UNUSED_D0;
    pinInit.mode              = IO::eMode::IO_MODE_INPUT;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> exti_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_EXTI, pinInit);
    auto pin = std::dynamic_pointer_cast<ExtiPin>(exti_pin);

    pin->EnableInterrupt(InterruptLed);
    pin->SelectInterruptTrigger(
        IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE);

    while (1)
    {
    }
}
