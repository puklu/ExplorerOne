#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "Trace.hpp"
#include "printf.h"


UsartPin* Trace::printUsartPin = nullptr;
bool Trace::isInitialized = false;

void Trace::initialize()
{
    if(!isInitialized)
    {
        UsartPinInitStruct pinInit = {};
        pinInit.pin_name           = IO::ePin::IO_UART4_TX_PRINT;
        pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
        pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

        [[maybe_unused]] PinBase const *usart_pin =
            PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_USART, pinInit);

        isInitialized = true;
    }  
}

void Trace::print(const char* format, ...)
{
    initialize();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
void Trace::printWithMetadata(const char* module, const char* format, ...)
{
    initialize();

    if (module) {
        printf("[%s] ", module );  // Prefix with module name
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void Trace::printWithDetails(const char* file, int line, const char* format, ...)
{
    initialize();

    // Print file name and line number as a prefix
    printf("[%s:%d] ", file, line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
