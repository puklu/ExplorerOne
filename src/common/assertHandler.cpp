#include "assertHandler.hpp"
#include "delay.hpp"
#include "Trace.hpp"


void assertHandler(const char* file, int line)
{
    // TODO: Improve me
    
    TRACE_LOG("Assertion failed in file %s at line %d", file, line);

    SOFTWARE_BREAKPOINT
}