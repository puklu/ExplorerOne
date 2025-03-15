#include "BasicTimer.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"


BasicTimer::BasicTimer(BasicTimerConfig  const &timer_init_struct):
    BaseTimer(timer_init_struct.prescaler_value, timer_init_struct.auto_reload_register_value, timer_init_struct.cb)
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

eGeneralStatus BasicTimer::SetPeriodAndCount(Milliseconds period, uint32_t count)
{
    ASSERT(mpTimer);
    ASSERT(period.value <= ((float(UINT16_MAX)/SYS_CLK)*1000));
    ASSERT(count <= UINT16_MAX);
    
    mPrescalerValue = (period.value*SYS_CLK)/1000 - 1;
    SetPrescalerValue();

    mAutoReloadRegisterValue = count;
    SetAutoReloadRegisterValue();

    mPeriodOfCounterClockSeconds.value = period.value/1000;
    mPeriodOfCounterClockMilliSeconds.value = period.value;
    mPeriodOfCounterClockMicroSeconds.value = period.value*1000;
    
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::EnableInterrupt()
{
    EnableDmaAndInterrupt();
    EnableNVIC();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableInterrupt()
{
    DisableDmaAndInterrupt();

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

eGeneralStatus BasicTimer::EnableDmaAndInterrupt()
{
    ASSERT(mpTimer);

    // enable DMA request
    SetBits(mpTimer->DIER, 1<<8);
    
    // enable interrupts
    SetBits(mpTimer->DIER, 1<<0);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableDmaAndInterrupt()
{
    ASSERT(mpTimer);

    // disable DMA request
    ResetBits(mpTimer->DIER, 1<<8);

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


eGeneralStatus BasicTimer::SetAutoReloadRegisterValue()
{
    ASSERT(mpTimer);
    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
}
