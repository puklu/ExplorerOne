#include "BasicTimer.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"



BasicTimer::BasicTimer(TimerInitStruct  const &timer_init_struct):
    mPrescalerValue(timer_init_struct.prescaler_value),
    mAutoReloadRegisterValue(timer_init_struct.auto_reload_register_value),
    mCallBack(timer_init_struct.cb)
{
    ASSERT(mPrescalerValue < UINT16_MAX);

    for(uint8_t i=0; i< NUMBER_OF_BASIC_TIMERS; i++)
    {
        if(basicTimers[i] == nullptr)
        {
            mpTimer = aBasicTimersAddress[i];

            // enable the clock
            mpRCC->APB1ENR |= aBasicTimersEnableMasks[i];

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
    mpTimer->CR1 &= ~(1<<0);

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

    
void BasicTimer::EnableNVIC()
{
    NVIC_EnableIRQ(mIrqNumber);
    NVIC_SetPriority(mIrqNumber, PRIORITY_TIMER);  ///TODO: fix this priority
}


eGeneralStatus BasicTimer::SetControlRegisters()
{
    ASSERT(mpTimer);

    // enable ARPE
    mpTimer->CR1 |= 1<<7;
    
    // update request source
    mpTimer->CR1 |= 1<<2;
    
    // enable update event
    mpTimer->CR1 &= ~(1<<1);
    
    // enable the timer
    mpTimer->CR1 |= 1<<0;

    return eGeneralStatus::SUCCESS;

}

void BasicTimer::TriggerUpdateEvent()
{
    ASSERT(mpTimer);

    mpTimer->EGR |= 1 << 0; // Manually trigger update generation
}

eGeneralStatus BasicTimer::EnableDmaAndInterrupt()
{
    ASSERT(mpTimer);

    // enable DMA request
    mpTimer->DIER |= 1<<8;
    
    // enable interrupts
    mpTimer->DIER |= 1<<0;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableDmaAndInterrupt()
{
    ASSERT(mpTimer);

    // disable DMA request
    mpTimer->DIER &= ~(1<<8);

    // disable interrupts
    mpTimer->DIER &= ~(1<<0);

    return eGeneralStatus::SUCCESS;
}

InterruptCallback BasicTimer::GetInterruptCallback()
{
    return mCallBack;
}

eGeneralStatus BasicTimer::ClearInterrupt()
{
    ASSERT(mpTimer);

    mpTimer->SR &= ~(1 << 0); // Clear UIF

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus BasicTimer::SetPrescalerValue()
{
    ASSERT(mpTimer);
    ASSERT(mPrescalerValue >= 1 && mPrescalerValue <= 0xffff);
    mpTimer->PSC = mPrescalerValue;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::SetAutoReloadRegisterValue()
{
    ASSERT(mpTimer);
    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
}

float BasicTimer::GetSysClockTicksElapsed() const
{
    return GetCounterValue()/mPrescalerValue;
}

Microseconds BasicTimer::GetTimeElapsedInMicroseconds() const
{
    Microseconds us;
    us = GetTimeElapsed(mPeriodOfCounterClockMicroSeconds);
    return us;
}

Milliseconds BasicTimer::GetTimeElapsedInMilliseconds() const
{
    Milliseconds ms;
    ms = GetTimeElapsed(mPeriodOfCounterClockMilliSeconds);
    return ms;
}

template<typename TimeUnit>
TimeUnit BasicTimer::GetTimeElapsed(const TimeUnit& period) const
{
    TimeUnit time;
    if(mNumberOfTimesHighestValueReached != 0)
    {
        time.value = period.value * (mAutoReloadRegisterValue * mNumberOfTimesHighestValueReached + GetCounterValue());
    }
    else
    {
        time.value = period.value * GetCounterValue();
    }

    return time;
}

uint16_t BasicTimer::GetCounterValue() const
{
    return mpTimer->CNT;
}

void BasicTimer::IncrementNumberOfTimesHighestValueReached()
{
    mNumberOfTimesHighestValueReached++;
}

