#include "common/defines.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"
#include "SysTickImpl.hpp"


// initialize static members
SysTickImpl* SysTickImpl::mpInstance = nullptr;
volatile uint64_t SysTickImpl::mTicks = 0;

SysTickImpl::SysTickImpl()
    : mpSystick(aSysTick::ADDRESS)
{
}

SysTickImpl* SysTickImpl::GetInstance()
{
    if(mpInstance == nullptr)
    {
        mpInstance = new SysTickImpl();
    }

    return mpInstance;
}
 

eGeneralStatus SysTickImpl::SystickSetup(uint32_t freq, uint32_t ahb_clock) 
{
    SystickSetInterruptFrequency(freq, ahb_clock);
    SystickCounterEnable();
    SystickInterruptEnable();

    return eGeneralStatus::SUCCESS;
};

eGeneralStatus SysTickImpl::SystickDelay(Milliseconds delay) 
{
    volatile uint64_t start_time = GetTicks();
    while(GetTicks()-start_time < delay);

    return eGeneralStatus::SUCCESS;
};

void SysTickImpl::SystickSetInterruptFrequency(uint32_t freq, uint32_t ahb_clock)
{
    uint32_t ratio = ahb_clock / freq;
    mpSystick->LOAD = ratio - 1;
    mpSystick->VAL = 0;
}

void SysTickImpl::SystickCounterEnable()
{
    mpSystick->CTRL |= aSysTick::CTRL::CTRL_CLKSOURCE | aSysTick::CTRL::CTRL_ENABLE;
}

void SysTickImpl::SystickInterruptEnable()
{
    mpSystick->CTRL |= aSysTick::CTRL::CTRL_TICKINT;
}

uint64_t SysTickImpl::GetTicks()
{
    return mTicks;
}


extern "C" void SysTick_Handler()
{
    SysTickImpl:: mTicks++;
}
