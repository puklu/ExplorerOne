#include "BasicTimer.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"


BasicTimer::BasicTimer(BasicTimerConfig  const &timer_config):
    BaseTimer(timer_config.mPrescalerValue, timer_config.mAutoReloadRegisterValue, timer_config.mCb)
{
    ASSERT(mPrescalerValue < UINT16_MAX);

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
            break;
        }
        TRACE_LOG("No slot found");
    }

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();
    
    mIs32bitTimer = false;
    mIsInitialized = true;

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
    }
}

eGeneralStatus BasicTimer::Start()
{
    ASSERT(mIsInitialized);

    SetControlRegisters();
    EnableInterrupt();
    
    mIsTimerRunning = true;

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus BasicTimer::Stop()
{
    ASSERT(mpTimer);

    // disable the timer
    ResetBits(mpTimer->CR1, 1<<0);

    return eGeneralStatus::SUCCESS;
    
}

eGeneralStatus BasicTimer::Reset()
{
    TriggerUpdateEvent();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::EnableInterrupt()
{
    EnableInterrupt();
    EnableNVIC();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableInterrupt()
{
    DisableInterrupts();

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus BasicTimer::SetControlRegisters()
{
    ASSERT(mpTimer);

    // enable ARPE
    SetBits(mpTimer->CR1, 1<<7);
    
    // update request source
    SetBits(mpTimer->CR1, 1<<2);
    
    // enable update event
    ResetBits(mpTimer->CR1, 1<<1);
    
    // enable the timer
    SetBits(mpTimer->CR1, 1<<0);

    return eGeneralStatus::SUCCESS;

}

void BasicTimer::TriggerUpdateEvent()
{
    ASSERT(mpTimer);

    SetBits(mpTimer->EGR, 1<<0); // Manually trigger update generation
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
    return mCallBack;
}

eGeneralStatus BasicTimer::ClearInterrupt()
{
    ASSERT(mpTimer);

    ResetBits(mpTimer->SR, 1<<0); // Clear UIF

    return eGeneralStatus::SUCCESS;
}
