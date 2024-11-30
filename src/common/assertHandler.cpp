#include "assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "delay.hpp"
#include "printf.h"
#include "printf_redirect.h"
#include "Trace.hpp"



void assertHandler(const char* file, int line)
{
    // TODO: Improve me

    // printf_set_putchar(UsartPutchar);
    ActivateTraceForAssert();
    TRACE_LOG("Assertion failed in file %s at line %d", file, line);
    
    SOFTWARE_BREAKPOINT
}