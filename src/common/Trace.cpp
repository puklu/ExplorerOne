#include "drivers/interfaces/pinBank.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "Trace.hpp"
#include "printf.h"
#include "printf_redirect.h"


UsartPin* Trace::printUsartPin = nullptr;
bool Trace::isInitialized = false;

void Trace::initialize()
{
    isInitialized = true;  
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
