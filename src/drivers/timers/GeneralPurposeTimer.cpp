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
        
        if(i>0)
        {
            ASSERT(mAutoReloadRegisterValue < 0xFFFF); // only TIM2 has a 32 bit register
        }

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

    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        ChannelConfig channel = timer_config.mChannels[i];

        mChannels[i] = channel;
    }

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();

    ConfigureCaptureCompareRegisters();

    mIsInitialized = true;

}

eGeneralStatus GeneralPurposeTimer::Start()
{
    ASSERT(mIsInitialized);

    TriggerUpdateEvent();

    SetControlRegisters();

    EnableInterrupt();

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::Stop()
{
    // disable the timer
    mpTimer->CR1 &= ~(1<<0);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::Reset()
{
    TriggerUpdateEvent();

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::SetPeriod(uint32_t period_in_ms)
{
    ASSERT(mpTimer);
    // ASSERT(period_in_ms <= (float(UINT16_MAX)/SYS_CLK)*1000);
  
    mPrescalerValue = (period_in_ms*SYS_CLK)*1000 - 1;
    SetPrescalerValue();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetDutyCycle(volatile uint32_t *ccr_register, uint32_t period_in_ms, uint32_t duty_cycle)
{
    ASSERT(mpTimer);
  
    uint32_t ccr_value = ((period_in_ms*duty_cycle)/100)*(SYS_CLK/mPrescalerValue);

    *ccr_register |= ccr_value;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetPeriodAndCount(uint32_t period_in_ms, uint32_t count)
{
    SetPeriod(period_in_ms);

    mAutoReloadRegisterValue = count;
    SetAutoReloadRegisterValue();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetPeriodAndDutyCycle(volatile uint32_t *ccr_register, uint32_t period_in_ms, uint32_t duty_cycle)
{
    // SetPeriod(period_in_ms);

    // SetDutyCycle(ccr_register, period_in_ms, duty_cycle);

    // set period
    mPrescalerValue = (period_in_ms*SYS_CLK)/(float(UINT16_MAX)*1000);
    SetPrescalerValue();

    mAutoReloadRegisterValue = (period_in_ms*SYS_CLK)/((float(mPrescalerValue)+1)*1000);
    SetAutoReloadRegisterValue();

    uint32_t ccr_value = (float(duty_cycle)/100)*(mAutoReloadRegisterValue);

    *ccr_register |= ccr_value;


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
    DisableDmaAndInterrupt();
    
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


eGeneralStatus GeneralPurposeTimer::ConfigureCaptureCompareRegisters()
{
    ASSERT(mpTimer);

    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        // ChannelConfig channel = timer_config.mChannels[i];

        ChannelConfig channel = mChannels[i];

        // no need to proceed if a channel has not been assigned
        if(channel.mpChannelPin == nullptr)
        {
            continue;
        }

        uint8_t channel_index = i;
        volatile uint32_t *ccmr_register = nullptr;
        volatile uint32_t *ccr_register = nullptr;

        ccmr_register = (i<2) ? &mpTimer->CCMR1 : &mpTimer->CCMR2;

        switch (i)
        {
        case 0:
            ccr_register = &mpTimer->CCR1;
            
            break;

        case 1:
            // ASSERT(channel.mCaptureCompareValue < 0xFFFF); // because they are only 16 bits in this case
            ccr_register = &mpTimer->CCR2;
            break;            

        case 2:
            // ASSERT(channel.mCaptureCompareValue < 0xFFFF);  // because they are only 16 bits in this case
            ccr_register = &mpTimer->CCR3;
            break; 

        case 3:
            // ASSERT(channel.mCaptureCompareValue < 0xFFFF);  // because they are only 16 bits in this case
            ccr_register = &mpTimer->CCR4;
            break; 

        default:
            break;
        }


        ASSERT(ccmr_register); // assert that it is not still a nullptr at this point
        ASSERT(ccr_register); // assert that it is not still a nullptr at this point

        // set the capture/compare register value
        // *ccr_register |= channel.mCaptureCompareValue;

        SetAlternateFunction(channel);

        ConfigureCaptureCompareSelection(ccmr_register, channel.mSelection, channel_index);

        switch (channel.mSelection)
        {
            case Timer::eCaptureCompareSelection::NOT_SELECTED:
                TRACE_LOG("Channel selection not set");
                ASSERT(0);
                break;

            case Timer::eCaptureCompareSelection::OUTPUT:
                ConfigureOutputComparePreloadEnable(ccmr_register, channel.mOutputCompareConfig.mOutputComparePreloadEnable, channel_index);
                ConfigureOutputCompareMode(ccmr_register, channel.mOutputCompareConfig.mOutputCompareMode, channel_index);
                EnableOutputCompare(channel.mCaptureCompareEnable, channel_index);

                if(channel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_1 || 
                    channel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_2 )
                    {
                        SetPeriodAndDutyCycle(ccr_register, channel.mOutputCompareConfig.mPwmPeriodMs, channel.mOutputCompareConfig.mPwmDutyCyclePercent);
                    }

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
                ConfigureInputCapturePrescaler(ccmr_register, channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(ccmr_register, channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
                EnableInputCapture(channel.mCaptureCompareEnable, channel_index);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
                ConfigureInputCapturePrescaler(ccmr_register, channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(ccmr_register, channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
                EnableInputCapture(channel.mCaptureCompareEnable, channel_index);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
                ConfigureInputCapturePrescaler(ccmr_register, channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(ccmr_register, channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
                EnableInputCapture(channel.mCaptureCompareEnable, channel_index);

                break;                

            default:
                break;
        }        
    }
    
    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::EnableOutputCompare(Timer::eCaptureCompare enable, uint8_t channel_index)
{

    switch (enable)
    {
    case Timer::eCaptureCompare::ENABLE:

        // enable
        mpTimer->CCER |= aGeneralPurposeTimerCcerRegisterMasks[channel_index][0];
       
        // configure CCxNP
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][2];
       
        // configure CCxP
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][1];

        break;
    
    case Timer::eCaptureCompare::DISABLE:
        // disable
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][0];
        break;

    default:
        return eGeneralStatus::FAILURE;
        
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::EnableInputCapture(Timer::eCaptureCompare enable, uint8_t channel_index)
{

    switch (enable)
    {
    case Timer::eCaptureCompare::ENABLE:

        // enable
        mpTimer->CCER |= aGeneralPurposeTimerCcerRegisterMasks[channel_index][0];

        // TODO: Implemented only non-inverted rising edge for now
       
        // configure CCxNP
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][2];
       
        // configure CCxP
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][1];

        break;
    
    case Timer::eCaptureCompare::DISABLE:
        // disable
        mpTimer->CCER &= ~aGeneralPurposeTimerCcerRegisterMasks[channel_index][0];
        break;

    default:
        return eGeneralStatus::FAILURE;
        
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::ConfigureCaptureCompareSelection(volatile uint32_t *ccmr_register, Timer::eCaptureCompareSelection selection, uint8_t channel_index)
{
    ASSERT(ccmr_register);
    ASSERT(channel_index < GENERAL_PURPOSE_TIMER_NUM_CHANNELS);
    ASSERT(selection >= Timer::eCaptureCompareSelection::OUTPUT && selection <= Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC);

    switch (selection)
    {
    case Timer::eCaptureCompareSelection::OUTPUT:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][2];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][1];
        break;

    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;
    
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;

    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;

    default:
        ASSERT(0);
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}        


eGeneralStatus GeneralPurposeTimer::ConfigureInputCapturePrescaler(volatile uint32_t *ccmr_register, Timer::eInputCapturePrescaler prescaler, uint8_t channel_index)
{
 // configure prescaler for input
    switch (prescaler)
    {
    case Timer::eInputCapturePrescaler::NO_PRESCALER:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;
    
    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_2_EVENTS:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_4_EVENTS:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_8_EVENTS:
        *ccmr_register |= aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *ccmr_register |= aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    default:
        return eGeneralStatus::FAILURE;
    }
    
    return eGeneralStatus::SUCCESS;  
}


eGeneralStatus GeneralPurposeTimer::ConfigureInputCaptureFilter(volatile uint32_t *ccmr_register, Timer::eInputCaptureFilter filter, uint8_t channel_index)
{
    switch (filter)
    {
    case Timer::eInputCaptureFilter::NO_FILTER:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];
        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_2:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];
        break;    
    
    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_4:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_8:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_6:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_8:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_6:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_8:
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_6:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_8:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_5:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_6:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_8:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_5:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_6:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_8:
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *ccmr_register |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break;                                                                                 

    default:
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}    


eGeneralStatus GeneralPurposeTimer::ConfigureOutputCompareMode(volatile uint32_t *ccmr_register, Timer::eOutputCompareMode mode, uint8_t channel_index)
{                
    // configure the mode
    switch (mode)
    {
        case Timer::eOutputCompareMode::FROZEN: 
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::SET_TO_ACTIVE_LEVEL_ON_MATCH:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::SET_TO_INACTIVE_LEVEL_ON_MATCH:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];            
            break;

        case Timer::eOutputCompareMode::TOGGLE:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::FORCE_INACTIVE_LEVEL:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;    

        case Timer::eOutputCompareMode::FORCE_ACTIVE_LEVEL:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;    

        case Timer::eOutputCompareMode::PWM_MODE_1:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::PWM_MODE_2:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_1:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_2:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_1:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_2:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_1:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_2:
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *ccmr_register |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;               
        
        default:
            return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}                


eGeneralStatus GeneralPurposeTimer::ConfigureOutputComparePreloadEnable(volatile uint32_t *ccmr_register, Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index)
{
    switch (preload_enable)
    {
        case Timer::eOutputComparePreloadEnable::DISABLE:
            *ccmr_register &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];
            break;
        case Timer::eOutputComparePreloadEnable::ENABLE:
            *ccmr_register |= aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];
            break;  
        default:
            return eGeneralStatus::FAILURE;    
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetAlternateFunction(ChannelConfig channel_config)
{
    ASSERT(channel_config.mpChannelPin != nullptr);
    ASSERT(channel_config.mAlternateFunction != IO::eAlternateFunction::NONE);
    ASSERT(channel_config.mpChannelPin->GetMode() == IO::eMode::IO_MODE_ALT_FUNCTION);

    uint8_t       pinNumber = channel_config.mpChannelPin->GetPinNumber(); // todo: assert before this
    GPIO_TypeDef *mpPort = channel_config.mpChannelPin->GetPort(); // todo: assert before this
    IO::eAlternateFunction alternateFunction = channel_config.mAlternateFunction; // todo: assert before this

    if(pinNumber<8)
    {
        mpPort->AFR[0] &= ~(0xF << (pinNumber * 4));
        mpPort->AFR[0] |= (static_cast<uint8_t>(alternateFunction) << (pinNumber * 4));
    }
    else
    {
        uint8_t pin_number = pinNumber; // Making a copy to use here
        pin_number -= 8;

        mpPort->AFR[1] &= ~(0xF << (pin_number * 4));
        mpPort->AFR[1] |= (static_cast<uint8_t>(alternateFunction) << (pin_number * 4));
    }

    return eGeneralStatus::SUCCESS;
}


InterruptCallback GeneralPurposeTimer::GetInterruptCallback()
{
    return mCallBack;
}

ChannelConfig* GeneralPurposeTimer::GetChannels()
{
    return mChannels;
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
    
    // enable counter
    mpTimer->CR1 |= (1<<0);

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

    constexpr uint16_t bits_to_clear =  
        (1<<14) |
        (1<<12) |
        (1<<11) |
        (1<<10) |
        (1<<9)  |
        (1<<8)  | 
        (1<<6)  |
        (1<<4)  |
        (1<<3)  | 
        (1<<2)  |
        (1<<1)  |
        (1<<0);

    mpTimer->DIER &= ~bits_to_clear;

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
