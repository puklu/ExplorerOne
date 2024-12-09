#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "BasicTimer.hpp"
#include "common/Trace.hpp"



BasicTimer::BasicTimer(TimerInitStruct  const &timer_init_struct):
    mPrescalerValue(timer_init_struct.prescaler_value),
    mAutoReloadRegisterValue(timer_init_struct.auto_reload_register_value),
    mCallBack(timer_init_struct.cb)
{
    ASSERT(mPrescalerValue >= 0 && mPrescalerValue < UINT16_MAX);
    EnableClock();
    SetPrescalerValue();
    SetAutoReloadRegisterValue();

    for(uint8_t i=0; i< NUMBER_OF_BASIC_TIMERS; i++)
    {
        if(basicTimers[i] == nullptr)
        {
            basicTimers[i] = this;
            break;
        }
        TRACE_LOG("No slot found");
    }

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

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus BasicTimer::Stop()
{
    // disable the timer
    mpTimer->CR1 &= ~(1<<0);

    return eGeneralStatus::SUCCESS;
    
}

eGeneralStatus BasicTimer::Reset()
{
    TriggerUpdateEvent();

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus BasicTimer::SetPeriodAndCount(uint32_t period_in_seconds, uint32_t count)
{
    ASSERT(period_in_seconds <= ((float(UINT16_MAX)/SYS_CLK)*1000));
    ASSERT(count <= UINT16_MAX);
    
    mPrescalerValue = (period_in_seconds*SYS_CLK)/1000 - 1;
    mpTimer->PSC = mPrescalerValue;

    mAutoReloadRegisterValue = count;
    mpTimer->ARR = mAutoReloadRegisterValue;



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
    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, PRIORITY_TIMER);  ///TODO: fix this priority
}

eGeneralStatus BasicTimer::EnableClock() const
{
    mpRCC->APB1ENR |= 1<<5;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::SetControlRegisters()
{
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
    mpTimer->EGR |= 1 << 0; // Manually trigger update generation
}

eGeneralStatus BasicTimer::EnableDmaAndInterrupt()
{
    // enable DMA request
    mpTimer->DIER |= 1<<8;
    
    // enable interrupts
    mpTimer->DIER |= 1<<0;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::DisableDmaAndInterrupt()
{
    // disable DMA request
    mpTimer->DIER &= ~(1<<8);

    // enable interrupts
    mpTimer->DIER &= ~(1<<0);

    return eGeneralStatus::SUCCESS;
}

InterruptCallback BasicTimer::GetInterruptCallback()
{
    return mCallBack;
}

eGeneralStatus BasicTimer::ClearInterrupt()
{
    mpTimer->SR &= ~(1 << 0); // Clear UIF

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::ReadCounterValue()
{
    // TODO: Implement me
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::SetPrescalerValue()
{
    ASSERT(mPrescalerValue >= 1 && mPrescalerValue <= 0xffff);
    mpTimer->PSC = mPrescalerValue - 1;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BasicTimer::SetAutoReloadRegisterValue()
{
    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
}
