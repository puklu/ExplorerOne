#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "mcuInit.hpp"
#include "printf.h"

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    int i = 0;

    while (1)
    {
        TRACE_LOG("Hello from usart test %d", i);
        i++;
        DELAY(1000_ms);
    }
}
