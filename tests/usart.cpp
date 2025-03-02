#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "mcuInit.hpp"
#include "printf.h"

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    int i = 0;

    while (1)
    {
        TRACE_LOG("Hello from usart test %d", i);
        i++;
        delay(1000000);
    }
}
