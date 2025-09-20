#include "BasicTimer.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"


BasicTimer::BasicTimer(BasicTimerConfig  const &timer_config):
    BaseTimer(timer_config.mPrescalerValue, timer_config.mAutoReloadRegisterValue, timer_config.mCb),
    mrTimerConfig(timer_config)
{
    ASSERT(mPrescalerValue < UINT16_MAX);
}

eGeneralStatus BasicTimer::Init()
{
    bool freeTimerFound = false;

    for(uint8_t i=0; i< NUMBER_OF_BASIC_TIMERS; i++)
    {
        if(basicTimers[i] == nullptr)
        {
            mpTimer = aBasicTimersAddress[i];

            // enable the clock
            SetBits(mpRCC->APB1ENR, aBasicTimersEnableMasks[i]);

            // set IRQ number for the NVIC
            mIrqNumber = aBasicTimersIrqNumbers[i];

            // add the instance to the global array
            basicTimers[i] = this;

            freeTimerFound = true;

            break;
        }
    }
    
    if(!freeTimerFound)
    {
        TRACE_LOG("No slot found");
        ASSERT(false);
    }

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();

    
    mIs32bitTimer = false;
    mIsInitialized = true;

    SetPeriod(mrTimerConfig.mTimerClockPeriodMs);

    return eGeneralStatus::SUCCESS;
}

BasicTimer::~BasicTimer()
{
    
    for(uint8_t i=0; i< NUMBER_OF_BASIC_TIMERS; i++)
    {
        if(basicTimers[i] == this)
        {
            basicTimers[i] = nullptr;
            break;
        }
        TRACE_LOG("Basic timer destroyed");
    }
}

eGeneralStatus BasicTimer::Start()
{
    if(mIsTimerRunning)
    {
        return eGeneralStatus::SUCCESS;
    }

    ASSERT(mpTimer);
    ASSERT(mIsInitialized);

    SetControlRegisters();
    EnableInterrupt();
    
    mIsTimerRunning = true;

    TRACE_LOG("Timer started");

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus BasicTimer::Stop()
{
    ASSERT(mpTimer);

    if(!mIsTimerRunning)
    {
        return eGeneralStatus::SUCCESS;
    }

    // disable the timer
    ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::COUNTER_ENABLE));

    mIsTimerRunning = false;

    return eGeneralStatus::SUCCESS;
    
}

eGeneralStatus BasicTimer::Reset()
{
    ASSERT(mpTimer);

    TriggerUpdateEvent();

    mCountOfOverflows = 0;

    // TRACE_LOG("Timer reset");

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::EnableInterrupt()
{
    ASSERT(mpTimer);

    EnableInterrupts();
    EnableNVIC();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableInterrupt()
{
    ASSERT(mpTimer);

    DisableInterrupts();

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus BasicTimer::SetControlRegisters()
{
    ASSERT(mpTimer);

    // auto-preload
    switch (mrTimerConfig.mAutoReloadPreload)
    {
        case Timer::eAutoReloadPreload::ARR_BUFFERED:
            SetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::AUTO_RELOAD_PRELOAD_ENABLE));
            break;

        case Timer::eAutoReloadPreload::ARR_NOT_BUFFERED:
            ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::AUTO_RELOAD_PRELOAD_ENABLE));
            break;    
    
        default:
            TRACE_LOG("Something went wrong while enabling auto reload/preload");
            ASSERT(false);
    }
    
    // update request source
    switch (mrTimerConfig.mUpdateRequestSource)
    {
        case Timer::eUpdateRequestSource::ANY_EVENT:
            ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::UPDATE_REQUEST_SOURCE));
            break;
        
        case Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW:
            SetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::UPDATE_REQUEST_SOURCE));
            break;
        
        default:
            ASSERT(false);
    }
    
    // enable/disable update event
    switch (mrTimerConfig.mEnableUpdateEvent)
    {
        case Timer::eUpdateEvent::ENABLE_EVENT_GENERATION:
            ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::UPDATE_DISABLE));
            break;
        
        case Timer::eUpdateEvent::DISABLE_EVENT_GENERATION:
            SetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::UPDATE_DISABLE));
            break;
        
        default:
            ASSERT(false);
    }
    
    // enable the timer
    SetBits(mpTimer->CR1, 1<<0);

    return eGeneralStatus::SUCCESS;

}

void BasicTimer::TriggerUpdateEvent()
{
    ASSERT(mpTimer);

    SetBits(mpTimer->EGR, static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::UPDATE_GENERATION)); // Manually trigger update generation
}

eGeneralStatus BasicTimer::EnableDma()
{
    ASSERT(mpTimer);

    // enable DMA request
    SetBits(mpTimer->DIER, 1<<8);
    
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::EnableInterrupts()
{
    ASSERT(mpTimer);
    
    // enable interrupts
    SetBits(mpTimer->DIER, 1<<0);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableDma()
{
    ASSERT(mpTimer);

    // disable DMA request
    ResetBits(mpTimer->DIER, 1<<8);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableInterrupts()
{
    ASSERT(mpTimer);

    // disable interrupts
    ResetBits(mpTimer->DIER, 1<<0);

    return eGeneralStatus::SUCCESS;
}

InterruptCallback BasicTimer::GetInterruptCallback()
{
    ASSERT(mpTimer);

    return mCallBack;
}

eGeneralStatus BasicTimer::ClearInterrupt()
{
    ASSERT(mpTimer);

    ResetBits(mpTimer->SR, 1<<0); // Clear UIF

    return eGeneralStatus::SUCCESS;
}
