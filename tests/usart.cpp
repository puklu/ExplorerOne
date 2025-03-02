#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/interfaces/PinFactory.hpp"
// #include "drivers/io/GpioPin.hpp"
// #include "drivers/usart/UsartPin.hpp"
#include "mcuInit.hpp"
#include "printf.h"

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    // UsartPinInitStruct pinInit = {};
    // pinInit.pin_name           = IO::ePin::IO_UART4_TX_PRINT;
    // pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
    // pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

    // [[maybe_unused]] PinBase const *usart_print_pin = PinFactory::CreatePin(
    //     IO::ePinType::IO_PIN_TYPE_PRINTING_USART, pinInit);

    int i = 0;

    while (1)
    {
        TRACE_LOG("Hello from usart test %d", i);
        i++;
        delay(1000000);
    }
}
