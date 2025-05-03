#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"

#include <cstdint>  // for uint8_t

#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/time.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimerConfig.hpp"

int main()
{
    SystemInit();

    ASSERT(IsSystemInitialized());

    GeneralPurposeTimerConfig gptimer_config;
    gptimer_config.mUpdateRequestSource =
        Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;

    GeneralPurposeTimer timer(gptimer_config);
    timer.SetPeriod(1_ms);

    GpioPinInitStruct ledInit = {};
    ledInit.pin_name          = IO::ePin::IO_TEST_LED_LD4_BLUE;
    ledInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    ledInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    ledInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, ledInit);
    auto ledpin = std::dynamic_pointer_cast<IDigitalOutputPin>(gpio_pin);

    while (true)
    {
        ledpin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        timer.Reset();
        timer.Start();
        while (timer.GetTimeElapsedInMillisecondsSinceStart() <= 1000_ms)
            ;

        ledpin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        timer.Reset();
        timer.Start();
        while (timer.GetTimeElapsedInMillisecondsSinceStart() <= 1000_ms)
            ;
    }
}
