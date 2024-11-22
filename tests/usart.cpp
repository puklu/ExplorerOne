#include "common/delay.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "printf.h"

int main()
{
    UsartPinInitStruct pinInit = {};
    pinInit.pin_name           = IO::ePin::IO_UART4_TX_PRINT;
    pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
    pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

    [[maybe_unused]] PinBase const *usart_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_USART, pinInit);

    int i = 0;

    while (1)
    {
        printf("Hello from usart test %d\n", i);
        i++;
        delay(500000);
    }
}
