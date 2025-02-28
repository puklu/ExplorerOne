#include "assertHandler.hpp"
#include "drivers/stm32f3discovery/usart/UsartPin.hpp" // TODO: Get rid of by using an interface
#include "delay.hpp"
#include "printf.h"
#include "printf_redirect.h"
#include "Trace.hpp"



void assertHandler(const char* file, int line)
{
    ActivateTraceForAssert();
    TRACE_LOG("Assertion failed in file %s at line %d", file, line);
    
    SOFTWARE_BREAKPOINT
}