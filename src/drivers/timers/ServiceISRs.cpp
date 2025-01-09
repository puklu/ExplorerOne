#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/timers/BasicTimer.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"
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

void GeneralPurposeTimersServiceISR(uint8_t timer_index)
{
    GeneralPurposeTimer *tim = static_cast<GeneralPurposeTimer*>(generalPurposeTimers[timer_index]);
    
    if(tim == nullptr)
    {
        return; // exit if TIM doesn't exist
    }

    ChannelConfig *channels = tim->GetChannels();

    // handle interrupts of all the channels
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        ChannelConfig const &channel = channels[i];

        InterruptCallback callback = channel.mCaptureCompareCallbackFunction;
        if(callback != nullptr)
        {
            callback();
        }
    }
    
    tim->ClearInterrupt();
}
