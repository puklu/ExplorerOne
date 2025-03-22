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
        basicTimer = dynamic_cast<BasicTimer*>(basicTimers[i]);
        if(basicTimer != nullptr)
        {
            InterruptCallback callback = basicTimer->GetInterruptCallback();
            if(callback != nullptr){
               callback();
            }
            basicTimer->IncrementNumberOfTimesHighestValueReached();
            basicTimer->ClearInterrupt();
        }

    }	
}

void GeneralPurposeTimersServiceISR(uint8_t timer_index)
{
    GeneralPurposeTimer *tim = dynamic_cast<GeneralPurposeTimer*>(generalPurposeTimers[timer_index]);
    
    if(tim == nullptr)
    {
        return; // exit if TIM doesn't exist
    }

    // If Update interrupt occured
    if(tim->GetStatusRegister() & static_cast<uint32_t>(Timer::eStatusRegisterFlagsMasks::UPDATE_INTERRUPT_FLAG))
    {
        tim->IncrementNumberOfTimesHighestValueReached();
        tim->ClearInterrupt(Timer::eStatusRegisterFlagsMasks::UPDATE_INTERRUPT_FLAG);
    }

    std::vector<std::shared_ptr<ITimerChannel>> channels = tim->GetChannels();

    // a look-up table to find appropriate mask for each channel
    const std::array<Timer::eStatusRegisterFlagsMasks, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> captureCompareInterruptStatusMasks = {
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_1_INTERRUPT_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_2_INTERRUPT_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_3_INTERRUPT_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_4_INTERRUPT_FLAG
    };

    // a look-up table to find appropriate mask for each channel
    const std::array<Timer::eStatusRegisterFlagsMasks, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> captureCompareOvercaptureMasks = {
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_1_OVERCAPTURE_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_2_OVERCAPTURE_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_3_OVERCAPTURE_FLAG,
        Timer::eStatusRegisterFlagsMasks::CAPTURE_COMPARE_4_OVERCAPTURE_FLAG
    };
    
    // handle interrupts of all the channels
    for(uint8_t i=0; i<channels.size(); i++)
    {   
        if(tim->GetStatusRegister() & static_cast<uint32_t>(captureCompareInterruptStatusMasks[i]))
        {
            TimerChannel const &channel = *std::dynamic_pointer_cast<TimerChannel>(channels[i]);
    
            InterruptCallback callback = channel.mCaptureCompareCallbackFunction;
            if(callback != nullptr)
            {
                callback();
            }
            tim->ClearInterrupt(captureCompareInterruptStatusMasks[i]);
            tim->ClearInterrupt(captureCompareOvercaptureMasks[i]);
        }
    }

    // If trigger interrupt occured
    if(tim->GetStatusRegister() & static_cast<uint32_t>(Timer::eStatusRegisterFlagsMasks::TRIGGER_INTERRUPT_FLAG))
    {
        tim->ClearInterrupt(Timer::eStatusRegisterFlagsMasks::TRIGGER_INTERRUPT_FLAG);
    }

    // clear all interrupts
    tim->ClearInterrupt();
    
}
