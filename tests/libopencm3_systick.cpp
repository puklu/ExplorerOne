// This test is for testing/showing the use of systick functions
// from libopencm3 library

#include <libopencm3/cm3/systick.h>

#include <cstdint>  // for uint8_t

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/time.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

uint64_t        ticks = 0;
extern "C" void SysTick_Handler()
{
    ticks++;
}

uint64_t get_ticks()
{
    return ticks;
}

void SetUpSysTick()
{
    systick_set_frequency(1000, SYS_CLK);
    systick_counter_enable();
    systick_interrupt_enable();
}

void SystickDelay(uint64_t delay)
{
    volatile uint64_t start_time = get_ticks();
    while (get_ticks() - start_time < delay)
        ;
}

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    TRACE_LOG("Hello from main");

    SetUpSysTick();

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_TEST_LED_LD4_BLUE;
    pinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    pinInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    auto pin = std::dynamic_pointer_cast<IDigitalPin>(gpio_pin);

    while (true)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        SystickDelay(1000);

        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        SystickDelay(2000);
    }
}
