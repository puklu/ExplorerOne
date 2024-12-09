#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/timers/BasicTimer.hpp"
#include "ServiceISRs.hpp"


void BasicTimersServiceISR()
{
    BasicTimer *basicTimer = nullptr;

    for(uint8_t i=0; i< NUMBER_OF_BASIC_TIMERS; i++)
    {
        basicTimer = static_cast<BasicTimer*>(basicTimers[i]);
        if(basicTimer != nullptr)
        {
            InterruptCallback callback = basicTimer->GetInterruptCallback();
            if(callback != nullptr){
               callback();
            }
            basicTimer->ClearInterrupt();
        }

    }	
}
