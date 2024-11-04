// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/IrqHandlers.cpp"
#include "mcuInit.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "drivers/interfaces/PinFactory.hpp"


int main()
{

    SystemInit();

    UsartPinInitStruct pinInit = {};
    pinInit.pin_name           = IO::ePin::IO_UART4_TX;
    pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
    pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

    PinBase *usart_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_USART, pinInit);
    auto pin = static_cast<UsartPin *>(usart_pin);

    // char *data = 'Hello';
    // // uintptr_t data = myChar;

    // for(int i=0; i<sizeof(*data); i++){
    // USART::TransmitData(data++);
    // }

    while (1)
    {
        pin->TransmitData("d");
        pin->TransmitData("u");
        pin->TransmitData("c");
        pin->TransmitData("k");
        pin->TransmitData("\n");
        delay(500000);
    }
}
