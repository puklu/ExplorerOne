#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"
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

    std::array<std::shared_ptr<ITimerChannelConfig>, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> channels = tim->GetChannels();

    // handle interrupts of all the channels
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        ChannelConfig const &channel = *std::dynamic_pointer_cast<ChannelConfig>(channels[i]);

        InterruptCallback callback = channel.mCaptureCompareCallbackFunction;
        if(callback != nullptr)
        {
            callback();
        }
    }

    tim->ClearInterrupt();
}
