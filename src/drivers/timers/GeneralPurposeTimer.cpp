#include "common/assertHandler.hpp"
#include "common/AlternateFunctionsTable.hpp"
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

    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        ChannelConfig channel = timer_config.mChannels[i];

        if(channel.mpChannelPin)
        {
            mChannels[i] = channel;

            SelectTIM(channel.mpChannelPin, channel.mAlternateFunction);

            SetUpTimer();
        }
    }

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();

    ConfigureCaptureCompareRegisters();

    mIsInitialized = true;

}

eGeneralStatus GeneralPurposeTimer::SetUpTimer()
{
    uint8_t timer_index = GetTimerIndex();

    ASSERT(timer_index < NUMBER_OF_GENERAL_PURPOSE_TIMERS);

    if(timer_index == 0)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFFFFFF); // only TIM2 has a 32 bit register
        mIs32bitTimer = true;  //TIM2 is a 32 bit timer
    }

    else if(timer_index == 1)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFF);
    }

    else if(timer_index == 2)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFF);
    }

    // enable the clock
    mpRCC->APB1ENR |= aGeneralPurposeTimersEnableMasks[timer_index];

    // set IRQ number for the NVIC
    mIrqNumber = aGeneralPurposeTimersIrqNumbers[timer_index];

    // add the instance to the global array
    generalPurposeTimers[timer_index] = this;

    return eGeneralStatus::SUCCESS;

}

uint8_t GeneralPurposeTimer::GetTimerIndex()
{
    ASSERT(mpTimer);

    if(mpTimer == TIM2)
    {
        return 0;
    }
    
    else if(mpTimer == TIM3)
    {
        return 1;
    }

    else if(mpTimer == TIM4)
    {
        return 2;
    }

    ASSERT(0); // invalid timer

    return NUMBER_OF_GENERAL_PURPOSE_TIMERS; // fallback
}

eGeneralStatus GeneralPurposeTimer::SelectTIM(GpioPin *channel_pin, IO::eAlternateFunction af)
{

    ASSERT(channel_pin);
    
    const void *selectedTIM;
    
    uint8_t port_number =  channel_pin->GetPortNumber();
    uint8_t pin_number =  channel_pin->GetPinNumber();

    switch (port_number)
    {
    case 0:
        selectedTIM = aAltFunctionsAdressesPortA[pin_number][static_cast<uint8_t>(af)];
        break;
    
    case 1:
        selectedTIM = aAltFunctionsAdressesPortB[pin_number][static_cast<uint8_t>(af)];
        break;
    
    case 2:
        selectedTIM = aAltFunctionsAdressesPortC[pin_number][static_cast<uint8_t>(af)];
        break;
    
    case 3:
        selectedTIM = aAltFunctionsAdressesPortD[pin_number][static_cast<uint8_t>(af)];
        break;
    
    case 4:
        selectedTIM = aAltFunctionsAdressesPortE[pin_number][static_cast<uint8_t>(af)];
        break;
    
    case 5:
        selectedTIM = aAltFunctionsAdressesPortF[pin_number][static_cast<uint8_t>(af)];
        break;
    
    default:
        ASSERT(0);
        break;
    }

    if(mpTimer != nullptr && selectedTIM != mpTimer)
    {
        TRACE_LOG("All of the channels don't belong to the same TIM");
        ASSERT(0);
    }

    if(selectedTIM != nullptr)
    {
        mpTimer = const_cast<TIM_TypeDef*>(reinterpret_cast<const TIM_TypeDef*>(selectedTIM));
    }
    else
    {
        ASSERT(0);
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::Start()
{

    if(mIsTimerRunning)
    {
        return eGeneralStatus::SUCCESS;
    }

    ASSERT(mIsInitialized);

    TriggerUpdateEvent();

    SetControlRegisters();

    EnableInterrupt();

    mIsTimerRunning = true;

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::Stop()
{
    // disable the timer
    mpTimer->CR1 &= ~(1<<0);

    mIsTimerRunning = false;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::Reset()
{
    TriggerUpdateEvent();

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::SetPeriodAndCount(uint32_t period_in_ms, uint32_t count)
{
    ASSERT(mpTimer);
  
    mPrescalerValue = (period_in_ms*SYS_CLK)*1000 - 1;

    SetPrescalerValue();

    mAutoReloadRegisterValue = count;
    SetAutoReloadRegisterValue();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetPeriodAndDutyCycle(uint32_t period_in_ms, uint32_t duty_cycle, uint8_t channel_index)
{
    if(mIs32bitTimer)
    {
        mPrescalerValue = 1u;
    }
    else
    {
        mPrescalerValue = 122u; // to make sure that caluclated ARR value is within allowed range for a 16bit register
    }

    SetPrescalerValue();

    uint64_t numerator = static_cast<uint64_t>(period_in_ms)*SYS_CLK;
    double denominator = (static_cast<double>(mPrescalerValue)+1)*1000;

    mAutoReloadRegisterValue = numerator/denominator;
    SetAutoReloadRegisterValue();

    uint32_t ccr_value = (float(duty_cycle)/100)*(mAutoReloadRegisterValue);

    ASSERT(mChannels[channel_index].mCcrRegister != nullptr);

    *(mChannels[channel_index].mCcrRegister) = ccr_value;

    // TriggerUpdateEvent();

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

    if(mIs32bitTimer)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFFFFFF);
    }
    else
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFF);
    }

    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::ConfigureCaptureCompareRegisters()
{
    ASSERT(mpTimer);

    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        ChannelConfig &channel = mChannels[i];

        // no need to proceed if a channel has not been assigned
        if(channel.mpChannelPin == nullptr)
        {
            continue;
        }

        uint8_t channel_index = i;

        channel.mCcmrRegister = (i<2) ? &mpTimer->CCMR1 : &mpTimer->CCMR2;

        switch (i)
        {
        case 0:
            channel.mCcrRegister = &mpTimer->CCR1;
            
            break;

        case 1:
            channel.mCcrRegister = &mpTimer->CCR2;
            break;            

        case 2:
            channel.mCcrRegister = &mpTimer->CCR3;
            break; 

        case 3:
            channel.mCcrRegister = &mpTimer->CCR4;
            break; 

        default:
            break;
        }


        ASSERT(channel.mCcmrRegister); // assert that it is not still a nullptr at this point
        ASSERT(channel.mCcrRegister); // assert that it is not still a nullptr at this point

        SetAlternateFunction(channel);

        ConfigureCaptureCompareSelection(channel.mSelection, channel_index);

        switch (channel.mSelection)
        {
            case Timer::eCaptureCompareSelection::NOT_SELECTED:
                TRACE_LOG("Channel selection not set");
                ASSERT(0);
                break;

            case Timer::eCaptureCompareSelection::OUTPUT:
                ConfigureOutputComparePreloadEnable(channel.mOutputCompareConfig.mOutputComparePreloadEnable, channel_index);
                ConfigureOutputCompareMode(channel.mOutputCompareConfig.mOutputCompareMode, channel_index);
                EnableOutputCompare(channel.mCaptureCompareEnable, channel_index);

                if(channel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_1 || 
                    channel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_2 )
                    {
                        SetPeriodAndDutyCycle(channel.mOutputCompareConfig.mPwmPeriodMs, channel.mOutputCompareConfig.mPwmDutyCyclePercent, channel_index);
                    }

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
                ConfigureInputCapturePrescaler(channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
                EnableInputCapture(channel.mCaptureCompareEnable, channel_index);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
                ConfigureInputCapturePrescaler(channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
                EnableInputCapture(channel.mCaptureCompareEnable, channel_index);

                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
                ConfigureInputCapturePrescaler(channel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
                ConfigureInputCaptureFilter(channel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
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

eGeneralStatus GeneralPurposeTimer::ConfigureCaptureCompareSelection(Timer::eCaptureCompareSelection selection, uint8_t channel_index)
{
    ASSERT(mChannels[channel_index].mCcmrRegister);
    ASSERT(channel_index < GENERAL_PURPOSE_TIMER_NUM_CHANNELS);
    ASSERT(selection >= Timer::eCaptureCompareSelection::OUTPUT && selection <= Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC);

    switch (selection)
    {
    case Timer::eCaptureCompareSelection::OUTPUT:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][2];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][1];
        break;

    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;
    
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;

    case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][2];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][1];
        break;

    default:
        ASSERT(0);
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}        


eGeneralStatus GeneralPurposeTimer::ConfigureInputCapturePrescaler(Timer::eInputCapturePrescaler prescaler, uint8_t channel_index)
{
 // configure prescaler for input
    switch (prescaler)
    {
    case Timer::eInputCapturePrescaler::NO_PRESCALER:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;
    
    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_2_EVENTS:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_4_EVENTS:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    case Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_8_EVENTS:
        *(mChannels[channel_index].mCcmrRegister) |= aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][5];
        *(mChannels[channel_index].mCcmrRegister) |= aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][4];
        break;

    default:
        return eGeneralStatus::FAILURE;
    }
    
    return eGeneralStatus::SUCCESS;  
}


eGeneralStatus GeneralPurposeTimer::ConfigureInputCaptureFilter(Timer::eInputCaptureFilter filter, uint8_t channel_index)
{
    switch (filter)
    {
    case Timer::eInputCaptureFilter::NO_FILTER:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];
        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_2:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];
        break;    
    
    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_4:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_6:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_6:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_6:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_5:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_6:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break;

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_5:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_6:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break; 

    case Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_8:
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][10];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][9];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][8];
        *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[channel_index][7];

        break;                                                                                 

    default:
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}    


eGeneralStatus GeneralPurposeTimer::ConfigureOutputCompareMode(Timer::eOutputCompareMode mode, uint8_t channel_index)
{                
    // configure the mode
    switch (mode)
    {
        case Timer::eOutputCompareMode::FROZEN: 
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::SET_TO_ACTIVE_LEVEL_ON_MATCH:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::SET_TO_INACTIVE_LEVEL_ON_MATCH:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];            
            break;

        case Timer::eOutputCompareMode::TOGGLE:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::FORCE_INACTIVE_LEVEL:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;    

        case Timer::eOutputCompareMode::FORCE_ACTIVE_LEVEL:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;    

        case Timer::eOutputCompareMode::PWM_MODE_1:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::PWM_MODE_2:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_1:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;  

        case Timer::eOutputCompareMode::OPM_MODE_2:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_1:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::COMBINED_PWM_MODE_2:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_1:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;   

        case Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_2:
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
            *(mChannels[channel_index].mCcmrRegister) |=  aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];
            break;               
        
        default:
            return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;
}                


eGeneralStatus GeneralPurposeTimer::ConfigureOutputComparePreloadEnable(Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index)
{
    switch (preload_enable)
    {
        case Timer::eOutputComparePreloadEnable::DISABLE:
            *(mChannels[channel_index].mCcmrRegister) &= ~aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];
            break;
        case Timer::eOutputComparePreloadEnable::ENABLE:
            *(mChannels[channel_index].mCcmrRegister) |= aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];
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

    channel_config.mpChannelPin->SetAlternateFunction(channel_config.mAlternateFunction);

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

    // TODO: Change based on passed configuration, hardcoded for now

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

bool GeneralPurposeTimer::GetIsTimerRunning() const
{
    return mIsTimerRunning;
}
