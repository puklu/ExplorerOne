#include "common/assertHandler.hpp"
#include "common/registerArrays.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "GeneralPurposeTimer.hpp"

GeneralPurposeTimer::GeneralPurposeTimer(GeneralPurposeTimerConfig  const &timer_config):
    mPrescalerValue(timer_config.mPrescalerValue),
    mAutoReloadRegisterValue(timer_config.mAutoReloadRegisterValue),
    mCallBack(timer_config.mCb)
{
    ASSERT(mPrescalerValue < UINT16_MAX);

    for(uint8_t i=0; i< NUMBER_OF_GENERAL_PURPOSE_TIMERS; i++)
    {
        if(generalPurposeTimers[i] == nullptr)
        {
            mpTimer = aGeneralPurposeTimersAddress[i];

            // enable the clock
            mpRCC->APB1ENR |= aGeneralPurposeTimersEnableMasks[i];

            // set IRQ number for the NVIC
            mIrqNumber = aGeneralPurposeTimersIrqNumbers[i];

            // add the instance to the global array
            generalPurposeTimers[i] = this;
            break;
        }
        TRACE_LOG("No slot found");
    }

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();

    // set mode
    SetMode(timer_config);
    
    mIsInitialized = true;

}

eGeneralStatus GeneralPurposeTimer::Start()
{
    ASSERT(mIsInitialized);

    SetControlRegisters();
    EnableInterrupt();

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::Stop()
{
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::Reset()
{
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetPeriodAndCount(uint32_t period_in_seconds, uint32_t count)
{
    (void)period_in_seconds;
    (void)count;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::EnableInterrupt()
{
    EnableDmaAndInterrupt();
    EnableNVIC();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::DisableInterrupt()
{
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetPrescalerValue()
{
    ASSERT(mpTimer);
    ASSERT(mPrescalerValue >= 1 && mPrescalerValue <= 0xffff);
    mpTimer->PSC = mPrescalerValue;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetAutoReloadRegisterValue()
{
    ASSERT(mpTimer);
    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::SetMode(GeneralPurposeTimerConfig const &timer_config)
{
    ASSERT(mpTimer);

    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        volatile uint32_t *ccmr_register = nullptr;

        if(i<2)
        {
            ccmr_register = &mpTimer->CCMR1;
        }
        else
        {
            ccmr_register = &mpTimer->CCMR2;
        }

        ASSERT(ccmr_register); // assert that it is not still a nullptr at this point

        switch (timer_config.mChannels[i].mSelection)
        {
            case Timer::eCaptureCompareSelection::NOT_SELECTED:
                break;

            case Timer::eCaptureCompareSelection::OUTPUT:

                // configure channel as output
                *ccmr_register &= ~(1<<1);
                *ccmr_register &= ~(1<<0);

                ConfigureOutputComparePreloadEnable(ccmr_register, timer_config.mChannels[i].mOutputCompareConfig.mOutputComparePreloadEnable);
                ConfigureOutputCompareMode(ccmr_register, timer_config.mChannels[i].mOutputCompareConfig.mOutputCompareMode);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
                // configure channel as input
                *ccmr_register &= ~(1<<1);
                *ccmr_register |=  (1<<0);

                ConfigureInputCapturePrescaler(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCapturePrescaler);
                ConfigureInputCaptureFilter(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCaptureFilter);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
                // configure channel as input
                *ccmr_register |=  (1<<1);
                *ccmr_register &= ~(1<<0);

                ConfigureInputCapturePrescaler(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCapturePrescaler);
                ConfigureInputCaptureFilter(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCaptureFilter);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
                // configure channel as input
                *ccmr_register |=  (1<<1);
                *ccmr_register |=  (1<<0);

                ConfigureInputCapturePrescaler(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCapturePrescaler);
                ConfigureInputCaptureFilter(ccmr_register, timer_config.mChannels[i].mInputCaptureConfig.mInputCaptureFilter);

                break;                

            default:
                break;
        }        
    }
    
    return eGeneralStatus::SUCCESS;
}

void GeneralPurposeTimer::ConfigureInputCapturePrescaler(volatile uint32_t *ccmr_register, Timer::eInputCapturePrescaler prescaler)
{
 // configure prescaler for input
    switch (prescaler)
    {
    case Timer::eInputCapturePrescaler::NO_PRESCALER:
        *ccmr_register &= ~(1<<3);
        *ccmr_register &= ~(1<<2);
        break;
    
    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_2_EVENTS:
        *ccmr_register &= ~(1<<3);
        *ccmr_register |=  (1<<2);
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_4_EVENTS:
        *ccmr_register |=  (1<<3);
        *ccmr_register &= ~(1<<2);
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_8_EVENTS:
        *ccmr_register |= (1<<3);
        *ccmr_register |= (1<<2);
        break;

    default:
        break;
    }    
}


void GeneralPurposeTimer::ConfigureInputCaptureFilter(volatile uint32_t *ccmr_register, Timer::eInputCaptureFilter filter)
{
    switch (filter)
    {
    case Timer::eInputCaptureFilter::NO_FILTER:
        *ccmr_register &= ~(0xF<<4);
        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_2:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x1<<4);
        break;    
    
    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_4:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x2<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x3<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_6:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x4<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x5<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_6:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x6<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x7<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_6:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x8<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0x9<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_5:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xA<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_6:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xB<<4);
        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xC<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_5:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xD<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_6:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xE<<4);
        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_8:
        *ccmr_register &= ~(0xF<<4);
        *ccmr_register |=  (0xF<<4);
        break;                                                                                 

    default:
        break;
    }
}    


void GeneralPurposeTimer::ConfigureOutputCompareMode(volatile uint32_t *ccmr_register, Timer::eOutputCompareMode mode)
{                
    // configure the mode
    switch (mode)
    {
        case Timer::eOutputCompareMode::FROZEN:
            *ccmr_register &= ~(0x7 << 4);
            break;

        case Timer::eOutputCompareMode::SET_TO_ACTIVE_LEVEL_ON_MATCH:
            *ccmr_register &= ~(0x7 << 4);
            *ccmr_register |=   0x1 << 4;
            break;

        case Timer::eOutputCompareMode::SET_TO_INACTIVE_LEVEL_ON_MATCH:
            *ccmr_register &= ~(0x7 << 4);
            *ccmr_register |=   0x2 << 5;            
            break;

        case Timer::eOutputCompareMode::TOGGLE:
            *ccmr_register &= ~(0x7 << 4);
            *ccmr_register |=   0x3 << 4;
            break;

        case Timer::eOutputCompareMode::FORCE_INACTIVE_LEVEL:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x4 << 4;
            break;    

        case Timer::eOutputCompareMode::FORCE_ACTIVE_LEVEL:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x5 << 4;
            break;    

        case Timer::eOutputCompareMode::PWM_MODE_1:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x6 << 4;
            break;

        case Timer::eOutputCompareMode::PWM_MODE_2:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x7 << 4;
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_1:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x8 << 4;
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_2:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0x9 << 4;
            break;

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_1:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0xC << 4;
            break;   

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_2:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0xD << 4;
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_1:
            *ccmr_register &= ~(0xF << 4);
            *ccmr_register |=   0xE << 4;
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_2:
            *ccmr_register |=   0xF << 4;
            break;               
        
        default:
            break;
    }
}                


void GeneralPurposeTimer::ConfigureOutputComparePreloadEnable(volatile uint32_t *ccmr_register, Timer::eOutputComparePreloadEnable preload_enable)
{
    switch (preload_enable)
    {
        case Timer::eOutputComparePreloadEnable::DISABLE:
            *ccmr_register &= ~(1<<3);
            break;
        case Timer::eOutputComparePreloadEnable::ENABLE:
            *ccmr_register |= 1<<3;
            break;    
    }
}


InterruptCallback GeneralPurposeTimer::GetInterruptCallback()
{
    return mCallBack;
}

eGeneralStatus GeneralPurposeTimer::ClearInterrupt()
{
    ASSERT(mpTimer);

    // clear capture compare 4 overcapture flag
    mpTimer->SR &= ~(1<<12);
    
    // clear capture compare 3 overcapture flag
    mpTimer->SR &= ~(1<<11);
    
    // clear capture compare 2 overcapture flag
    mpTimer->SR &= ~(1<<10);
    
    // clear capture compare 1 overcapture flag
    mpTimer->SR &= ~(1<<9);

    // clear trigger interrupt flag
    mpTimer->SR &= ~(1<<6);

    // clear capture compare 4 interrupt flag
    mpTimer->SR &= ~(1<<4);

    // clear capture compare 3 interrupt flag
    mpTimer->SR &= ~(1<<3);

    // clear capture compare 2 interrupt flag
    mpTimer->SR &= ~(1<<2);

    // clear capture compare 1 interrupt flag
    mpTimer->SR &= ~(1<<1);

    // clear update interrupt flag
    mpTimer->SR &= ~(1<<0);

    return eGeneralStatus::SUCCESS;
}

GeneralPurposeTimer::~GeneralPurposeTimer()
{
    for(uint8_t i=0; i< NUMBER_OF_GENERAL_PURPOSE_TIMERS; i++)
    {
        if(generalPurposeTimers[i] == this)
        {
            generalPurposeTimers[i] = nullptr;
            break;
        }
    }
}


eGeneralStatus GeneralPurposeTimer::SetControlRegisters()
{
    ASSERT(mpTimer);

    // TODO: Change based on passed configuration, hardcoded for now

    // clock division to zero
    mpTimer->CR1 &= ~(1<<9);
    mpTimer->CR1 &= ~(1<<8);

    // auto-preload
    mpTimer->CR1 |= 1<<7;

    // edge aligned mode
    mpTimer->CR1 &= ~(1<<6);
    mpTimer->CR1 &= ~(1<<5);

    // set direction
    mpTimer->CR1 &= ~(1<<4);

    // update request source
    mpTimer->CR1 &= ~(1<<2);

    // enable update event
    mpTimer->CR1 &= ~(1<<1);

    // TI1 selection
    mpTimer->CR2 &= ~(1<<7);

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::EnableDmaAndInterrupt()
{
    ASSERT(mpTimer);

    // enable trigger interrupt
    mpTimer->DIER |= 1<<6;

    // capture compare 4 interrupt enable
    mpTimer->DIER |= 1<<4;

    // capture compare 3 interrupt enable
    mpTimer->DIER |= 1<<3;

    // capture compare 2 interrupt enable
    mpTimer->DIER |= 1<<2;

    // capture compare 1 interrupt enable
    mpTimer->DIER |= 1<<1;

    // enable interrupt enable
    mpTimer->DIER |= 1<<0;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::DisableDmaAndInterrupt()
{
    return eGeneralStatus::SUCCESS;
}

void GeneralPurposeTimer::TriggerUpdateEvent()
{
    ASSERT(mpTimer);

    // trigger generation
    mpTimer->EGR |= 1<<6;

    // capture compare 4 generation
    mpTimer->EGR |= 1<<4;

    // capture compare 3 generation
    mpTimer->EGR |= 1<<3;

    // capture compare 2 generation
    mpTimer->EGR |= 1<<2;

    // capture compare 1 generation
    mpTimer->EGR |= 1<<1;

    // update generation
    mpTimer->EGR |= 1<<0;
}

void GeneralPurposeTimer::EnableNVIC()
{
    NVIC_EnableIRQ(mIrqNumber);
    NVIC_SetPriority(mIrqNumber, PRIORITY_TIMER);
}
