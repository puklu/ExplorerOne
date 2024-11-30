#include "assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "delay.hpp"
#include "printf.h"
#include "printf_redirect.h"
#include "Trace.hpp"



void assertHandler(const char* file, int line)
{
    // If the usart print pin has not been created, create it
    if(!activePrintUsartPin)
    {
        UsartPinInitStruct pinInit = {};
        pinInit.pin_name           = IO::ePin::IO_UART4_TX_PRINT;
        pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
        pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

        [[maybe_unused]] PinBase const *usart_print_pin =
            PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_PRINTING_USART, pinInit);
        
    }

    ActivateTraceForAssert();
    TRACE_LOG("Assertion failed in file %s at line %d", file, line);
    
    SOFTWARE_BREAKPOINT
}