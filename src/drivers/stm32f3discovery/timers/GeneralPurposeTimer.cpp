#include <numeric>
#include <unordered_map>
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/AlternateFunctionsTable.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"
#include "GeneralPurposeTimer.hpp"

GeneralPurposeTimer::GeneralPurposeTimer(GeneralPurposeTimerConfig const &timer_config):
    BaseTimer(timer_config.mPrescalerValue, timer_config.mAutoReloadRegisterValue, timer_config.mCb),
    mrTimerConfig(timer_config)
{
    ASSERT(mPrescalerValue < UINT16_MAX);

    TransferChannelsFromConfig(timer_config.mChannels);

    SelectTIM();

    SetUpTimer();

    // sets default value in case not provided by the user
    SetPrescalerValue();

    // sets default value in case not provided by the user
    SetAutoReloadRegisterValue();

    ConfigureCaptureCompareChannels();

    mIsInitialized = true;

}

eGeneralStatus GeneralPurposeTimer::TransferChannelsFromConfig(std::vector<std::shared_ptr<TimerChannel>> channels_in_config)
{
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {   
        // downcast the shared_ptr<ITimerChannel> to shared_ptr<TimerChannel>
        auto channel = std::move(channels_in_config[i]);

        if(channel->mpChannelPin)
        {
            mpChannels.push_back(std::move(channel));
        }
    }

    return eGeneralStatus::SUCCESS;
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

    else if(timer_index == 1 || timer_index == 2)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFF);
    }

    // enable the clock
    SetBits(mpRCC->APB1ENR, aGeneralPurposeTimersEnableMasks[timer_index]);

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

eGeneralStatus GeneralPurposeTimer::SelectTIM()
{
    // if none of the channels are being used, then select the next timer that is available
    if(mpChannels.size() == 0)
    {
        uint8_t timer_index_to_use = 0;
        for(uint8_t i =0; i<NUMBER_OF_GENERAL_PURPOSE_TIMERS; i++)
        {
            if(generalPurposeTimers[i] == nullptr)
            {
                timer_index_to_use = i;
                break;
            }
        }

        switch (timer_index_to_use)
        {
            case 0:
                mpTimer = TIM2;
                break;

            case 1:
                mpTimer = TIM3;
                break;
            
            case 2:
                mpTimer = TIM4;
                break;
        
            default:
                TRACE_LOG("No general purpose timer available");
                ASSERT(false);
                break;
        }
        return eGeneralStatus::SUCCESS;
    }

    for(auto channel: mpChannels)
    {
        if(channel == nullptr)
        {
            continue;
        }

        ASSERT(channel);

        auto channel_pin = channel->GetChannelPin();
        auto af = std::dynamic_pointer_cast<TimerChannel>(channel)->mAlternateFunction;

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

    }
    
    return eGeneralStatus::SUCCESS;

}


eGeneralStatus GeneralPurposeTimer::Start()
{

    if(mIsTimerRunning)
    {
        return eGeneralStatus::SUCCESS;
    }

    ASSERT(mpTimer);
    ASSERT(mIsInitialized);

    // TriggerUpdateEvent();

    SetControlRegisters();

    EnableInterrupt();

    mIsTimerRunning = true;

    TRACE_LOG("Timer started");

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::Stop()
{
    ASSERT(mpTimer);

    if(!mIsTimerRunning)
    {
        return eGeneralStatus::SUCCESS;
    }

    // disable the timer
    ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::COUNTER_ENABLE));

    mIsTimerRunning = false;

    TRACE_LOG("Timer stopped");

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::Reset()
{
    ASSERT(mpTimer);

    TriggerUpdateEvent();

    mCountOfOverflows = 0;

    TRACE_LOG("Timer reset");

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::SetDutyCycle(uint32_t duty_cycle, uint8_t channel_index)
{
    ASSERT(mpTimer);

    uint32_t ccr_value = (float(duty_cycle)/100)*(mAutoReloadRegisterValue);

    auto channel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    volatile uint32_t* ccrRegister = channel->mCcrRegister;

    ASSERT(ccrRegister != nullptr);

    *ccrRegister = ccr_value;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::EnableInterrupt()
{
    ASSERT(mpTimer);

    EnableInterrupts();
    EnableNVIC();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::DisableInterrupt()
{
    ASSERT(mpTimer);

    DisableInterrupts();
    
    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::ConfigureCaptureCompareChannels()
{
    ASSERT(mpTimer);

    // configure each channel in use one by one
    for(uint8_t channel_index = 0; channel_index < mpChannels.size(); channel_index++)
    {   
        ASSERT(mpChannels[channel_index] != nullptr);

        TimerChannel &rChannel =  *(std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]));
        
        FindCcmrAndCcrRegistersForChannel(rChannel, channel_index);

        SetAlternateFunctionForChannel(rChannel);

        SelectDirectionForChannel(rChannel.mSelection, channel_index);

        switch (rChannel.mSelection)
        {
            case Timer::eCaptureCompareSelection::NOT_SELECTED:
                TRACE_LOG("Channel direction not selected");
                ASSERT(0);
                break;

            case Timer::eCaptureCompareSelection::OUTPUT:
                ConfigureChannelForOutputCompareMode(rChannel, channel_index);
                break;

            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4:
            case Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC:
                ConfigureChannelForInputCaptureMode(rChannel, channel_index);
                break;                

            default:
                break;
        }        
    }
    
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::FindCcmrAndCcrRegistersForChannel(TimerChannel &rChannel, const uint8_t& channel_index)
{
    ASSERT(mpTimer);

    // no need to proceed if a channel has not been assigned
    if(rChannel.mpChannelPin == nullptr)
    {
        return eGeneralStatus::SUCCESS;
    }

    rChannel.mCcmrRegister = (channel_index<2) ? &mpTimer->CCMR1 : &mpTimer->CCMR2;

    switch (channel_index)
    {
    case 0:
        rChannel.mCcrRegister = &mpTimer->CCR1;
        break;

    case 1:
        rChannel.mCcrRegister = &mpTimer->CCR2;
        break;            

    case 2:
        rChannel.mCcrRegister = &mpTimer->CCR3;
        break; 

    case 3:
        rChannel.mCcrRegister = &mpTimer->CCR4;
        break; 

    default:
        break;
    }


    ASSERT(rChannel.mCcmrRegister); // assert that it is not still a nullptr at this point
    ASSERT(rChannel.mCcrRegister); // assert that it is not still a nullptr at this point
    
    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::EnableOutputCompare(Timer::eCaptureCompare enable, uint8_t channel_index)
{
    ASSERT(mpTimer);

    switch (enable)
    {
    case Timer::eCaptureCompare::ENABLE:

        // enable
        SetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][0]);
       
        // configure CCxNP
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][2]);
       
        // configure CCxP
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][1]);

        break;
    
    case Timer::eCaptureCompare::DISABLE:
        // disable
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][0]);
        break;

    default:
        return eGeneralStatus::FAILURE;
        
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::EnableInputCapture(Timer::eCaptureCompare enable, uint8_t channel_index)
{
    ASSERT(mpTimer);

    switch (enable)
    {
    case Timer::eCaptureCompare::ENABLE:

        // enable
        SetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][0]);

        // TODO: Implemented only non-inverted rising edge for now
       
        // configure CCxNP
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][2]);
       
        // configure CCxP
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][1]);

        break;
    
    case Timer::eCaptureCompare::DISABLE:
        // disable
        ResetBits(mpTimer->CCER, aGeneralPurposeTimerCcerRegisterMasks[channel_index][0]);
        break;

    default:
        return eGeneralStatus::FAILURE;
        
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::SelectDirectionForChannel(Timer::eCaptureCompareSelection selection, uint8_t channel_index)
{
    ASSERT(mpTimer);
    ASSERT(channel_index < GENERAL_PURPOSE_TIMER_NUM_CHANNELS);
    ASSERT(selection >= Timer::eCaptureCompareSelection::OUTPUT && selection <= Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC);

    auto pChannel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    // get the ccmr register pointer
    volatile uint32_t* pCcmrRegister = pChannel->mCcmrRegister;
    ASSERT(pCcmrRegister);

    // masks to modify the bits
    uint32_t mask2 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][2];
    uint32_t mask1 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][1];

    // clear all bits first
    ResetBits(*pCcmrRegister, mask2 | mask1);

    // a look-up table to find appropriate mask for each case
    const std::unordered_map<Timer::eCaptureCompareSelection, uint32_t> selectionMasks = {
        {Timer::eCaptureCompareSelection::OUTPUT, 0}, // no bits to set
        {Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI1, mask1},
        {Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI3, mask1},
        {Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI2, mask2},
        {Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TI4, mask2},
        {Timer::eCaptureCompareSelection::INPUT_AND_MAPPED_ON_TRC, mask2 | mask1},
    };

    // find the needed "selection" in the look-up table
    auto it = selectionMasks.find(selection);

    // found?
    if(it != selectionMasks.end())
    {
        SetBits(*pCcmrRegister, it->second);  // the value (for the key) from the lookup table used as mask
    }

    else
    {
        ASSERT(0);
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;

}        


eGeneralStatus GeneralPurposeTimer::ConfigureInputCapturePrescaler(Timer::eInputCapturePrescaler prescaler, uint8_t channel_index)
{
    ASSERT(mpTimer);

    auto pChannel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    // get the ccmr register pointer
    volatile uint32_t* pCcmrRegister = pChannel->mCcmrRegister;

    // masks to modify the bits
    uint32_t mask5 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][5];
    uint32_t mask4 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];

    // clear all bits first
    ResetBits(*pCcmrRegister, mask5 | mask4);


    // a look-up table to find appropriate mask for each case
    const std::unordered_map<Timer::eInputCapturePrescaler, uint32_t> prescalerMasks = {
        {Timer::eInputCapturePrescaler::NO_PRESCALER, 0}, // no bits to set
        {Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_2_EVENTS, mask4},
        {Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_4_EVENTS, mask5},
        {Timer::eInputCapturePrescaler::CAPTURE_ONCE_EVERY_8_EVENTS, mask5 | mask4}
    };


   // find the needed "prescaler" in the look-up table
   auto it = prescalerMasks.find(prescaler);

   // found?
   if(it != prescalerMasks.end())
   {
       SetBits(*pCcmrRegister, it->second);  // the value (for the key) from the lookup table used as mask
   }

   else
   {
       ASSERT(0);
       return eGeneralStatus::FAILURE;
   }

   return eGeneralStatus::SUCCESS;

}


eGeneralStatus GeneralPurposeTimer::ConfigureInputCaptureFilter(Timer::eInputCaptureFilter filter, uint8_t channel_index)
{
    ASSERT(mpTimer);

    auto pChannel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    // get the ccmr register pointer
    volatile uint32_t* pCcmrRegister = pChannel->mCcmrRegister;

    // masks to modify the bits
    uint32_t mask10 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][10];
    uint32_t mask9 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
    uint32_t mask8 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
    uint32_t mask7 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];

    // clear all bits first
    ResetBits(*pCcmrRegister, mask10 | mask9 | mask8 | mask7);


    // a look-up table to find appropriate mask for each case
    const std::unordered_map<Timer::eInputCaptureFilter, uint32_t> filterMasks = {
        {Timer::eInputCaptureFilter::NO_FILTER, 0},  // not bits to set
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_2, mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_4, mask8},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_CK_INT_AND_N_8, mask8 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_6, mask9},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_2_AND_N_8, mask9 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_6, mask9 | mask8},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_4_AND_N_8, mask9 | mask8 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_6, mask10},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_8_AND_N_8, mask10 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_5, mask10 | mask8},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_6, mask10 | mask8 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_16_AND_N_8, mask10 | mask9},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_5, mask10 | mask9 | mask7},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_6, mask10 | mask9 | mask8},
        {Timer::eInputCaptureFilter::F_SAMPLING_EQUALS_F_TDS_OVER_32_AND_N_8, mask10 | mask9 | mask8 | mask7},
    };


    // find the needed "prescaler" in the look-up table
    auto it = filterMasks.find(filter);

    // found?
    if(it != filterMasks.end())
    {
        SetBits(*pCcmrRegister, it->second);  // the value (for the key) from the lookup table used as mask
    }

    else
    {
    ASSERT(0);
    return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;

}    


eGeneralStatus GeneralPurposeTimer::ConfigureOutputCompareMode(Timer::eOutputCompareMode mode, uint8_t channel_index)
{
    ASSERT(mpTimer);

    auto pChannel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    // get the ccmr register pointer
    volatile uint32_t* pCcmrRegister = pChannel->mCcmrRegister;

    // masks to modify the bits
    uint32_t mask9 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][9];
    uint32_t mask8 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][8];
    uint32_t mask7 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][7];
    uint32_t mask6 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][6];

    // clear all bits first
    ResetBits(*pCcmrRegister, mask9 | mask8 | mask7 | mask6);


   // a look-up table to find appropriate mask for each case
    const std::unordered_map<Timer::eOutputCompareMode, uint32_t> modeMasks = {
        {Timer::eOutputCompareMode::FROZEN, 0},     // not bits to set
        {Timer::eOutputCompareMode::SET_TO_ACTIVE_LEVEL_ON_MATCH, mask6},
        {Timer::eOutputCompareMode::SET_TO_INACTIVE_LEVEL_ON_MATCH, mask7},
        {Timer::eOutputCompareMode::TOGGLE, mask7 | mask6},
        {Timer::eOutputCompareMode::FORCE_INACTIVE_LEVEL, mask8},
        {Timer::eOutputCompareMode::FORCE_ACTIVE_LEVEL, mask8 | mask6},
        {Timer::eOutputCompareMode::PWM_MODE_1, mask8 | mask7},
        {Timer::eOutputCompareMode::PWM_MODE_2, mask8 | mask7 | mask6},
        {Timer::eOutputCompareMode::OPM_MODE_1, mask9},
        {Timer::eOutputCompareMode::OPM_MODE_2, mask9 | mask6},
        {Timer::eOutputCompareMode::COMBINED_PWM_MODE_1, mask9 | mask8},
        {Timer::eOutputCompareMode::COMBINED_PWM_MODE_2, mask9 | mask8 | mask6},
        {Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_1, mask9 | mask8 | mask7},
        {Timer::eOutputCompareMode::ASYMMETRIC_PWM_MODE_2, mask9 | mask8 | mask7 | mask6},
    };

    
    // find the needed "mode" in the look-up table
    auto it = modeMasks.find(mode);
    
    // found?
    if(it != modeMasks.end())
    {
        SetBits(*pCcmrRegister, it->second);  // the value (for the key) from the lookup table used as mask
    }

    else
    {
        ASSERT(0);
        return eGeneralStatus::FAILURE;
    }

    return eGeneralStatus::SUCCESS;

}                


eGeneralStatus GeneralPurposeTimer::ConfigureChannelForOutputCompareMode(const TimerChannel &rChannel, const uint8_t& channel_index)
{
    ASSERT(mpTimer);

    ConfigureOutputComparePreloadEnable(rChannel.mOutputCompareConfig.mOutputComparePreloadEnable, channel_index);
    ConfigureOutputCompareMode(rChannel.mOutputCompareConfig.mOutputCompareMode, channel_index);
    EnableOutputCompare(rChannel.mCaptureCompareEnable, channel_index);

    if(rChannel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_1 || 
        rChannel.mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_2 )
    {
        SetPeriod(Milliseconds{rChannel.mOutputCompareConfig.mPwmPeriodMs});
        SetDutyCycle(rChannel.mOutputCompareConfig.mPwmDutyCyclePercent, channel_index);
    }

    return eGeneralStatus::SUCCESS;
}


eGeneralStatus GeneralPurposeTimer::ConfigureChannelForInputCaptureMode(const TimerChannel &rChannel, const uint8_t& channel_index)
{
    ASSERT(mpTimer);

    ConfigureInputCapturePrescaler(rChannel.mInputCaptureConfig.mInputCapturePrescaler, channel_index);
    ConfigureInputCaptureFilter(rChannel.mInputCaptureConfig.mInputCaptureFilter, channel_index);
    EnableInputCapture(rChannel.mCaptureCompareEnable, channel_index);

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::ConfigureOutputComparePreloadEnable(Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index)
{
    ASSERT(mpTimer);

    auto pChannel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[channel_index]);

    // get the ccmr register pointer
    volatile uint32_t* pCcmrRegister = pChannel->mCcmrRegister;

    // masks to modify the bits
    uint32_t mask4 = aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[channel_index][4];

    // clear all bits first
    ResetBits(*pCcmrRegister, mask4);

    switch (preload_enable)
    {
        case Timer::eOutputComparePreloadEnable::DISABLE:
            // no bits to set
            break;
        case Timer::eOutputComparePreloadEnable::ENABLE:
            SetBits(*pCcmrRegister, mask4);
            break;  
        default:
            return eGeneralStatus::FAILURE;    
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::SetAlternateFunctionForChannel(TimerChannel channel)
{
    ASSERT(channel.mpChannelPin != nullptr);
    ASSERT(channel.mAlternateFunction != IO::eAlternateFunction::NONE);
    ASSERT(std::dynamic_pointer_cast<GpioPin>(channel.mpChannelPin)->GetMode() == IO::eMode::IO_MODE_ALT_FUNCTION);

    std::dynamic_pointer_cast<GpioPin>(channel.mpChannelPin)->SetAlternateFunction(channel.mAlternateFunction);

    return eGeneralStatus::SUCCESS;
}


InterruptCallback GeneralPurposeTimer::GetInterruptCallback()
{
    ASSERT(mpTimer);

    return mCallBack;
}


std::vector<std::shared_ptr<ITimerChannel>> GeneralPurposeTimer::GetChannels()
{
    ASSERT(mpTimer);

    return mpChannels;
}

eGeneralStatus GeneralPurposeTimer::ClearInterrupt(Timer::eStatusRegisterFlagsMasks flagToClear)
{
    ASSERT(mpTimer);

    ResetBits(mpTimer->SR, static_cast<uint32_t>(flagToClear));

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

    // clock division
    uint32_t maskForCkdMsb = static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CLOCK_DIVISION_MSB);
    uint32_t maskForCkdLsb = static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CLOCK_DIVISION_LSB);
    uint32_t maskForCkdBoth = maskForCkdMsb | maskForCkdLsb;
    
    switch (mrTimerConfig.mClkDivisionForFilter)
    {
        case Timer::eClockDivisionForFilter::ONE:
            ResetBits(mpTimer->CR1, maskForCkdBoth);
            break;
        
        case Timer::eClockDivisionForFilter::TWO:
            ResetBits(mpTimer->CR1, maskForCkdMsb);
            SetBits(mpTimer->CR1, maskForCkdLsb);
            break;

        case Timer::eClockDivisionForFilter::FOUR:
            SetBits(mpTimer->CR1, maskForCkdMsb);
            ResetBits(mpTimer->CR1, maskForCkdLsb);
            break;
        
        default:
            break;
    }


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

    
    // mode alignment
    uint32_t maskForModeAlignmentMsb =
        static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CENTRE_ALIGNED_MODE_SELECTION_MSB);

    uint32_t maskForModeAlignmentLsb =
        static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CENTRE_ALIGNED_MODE_SELECTION_LSB);
        
    uint32_t maskForModeAlignmentBothBits =
        static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CENTRE_ALIGNED_MODE_SELECTION_MSB)|
        static_cast<uint32_t>(Timer::eControlRegister_1_Masks::CENTRE_ALIGNED_MODE_SELECTION_LSB);

    switch (mrTimerConfig.mModeAlignment)
    {
        case Timer::eModeAlignment::EDGE_ALIGNED:
            ResetBits(mpTimer->CR1, maskForModeAlignmentBothBits);
            break;

        case Timer::eModeAlignment::CENTRE_ALIGNED_MODE_1:
            ResetBits(mpTimer->CR1, maskForModeAlignmentMsb);
            SetBits(mpTimer->CR1, maskForModeAlignmentLsb);
            break;

        case Timer::eModeAlignment::CENTRE_ALIGNED_MODE_2:
            SetBits(mpTimer->CR1, maskForModeAlignmentMsb);
            ResetBits(mpTimer->CR1, maskForModeAlignmentLsb);
            break;
        
        case Timer::eModeAlignment::CENTRE_ALIGNED_MODE_3:
            SetBits(mpTimer->CR1, maskForModeAlignmentBothBits);
            break;
        
        default:
            ASSERT(false);
    }

    // set direction of counting
    switch (mrTimerConfig.mDirection)
    {
        case Timer::eDirection::UPCOUNTER:
            ResetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::DIRECTION));
            break;
        
        case Timer::eDirection::DOWNCOUNTER:
            SetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::DIRECTION));
            break;    
        
        default:
            break;
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


    // TI1 selection
    ResetBits(mpTimer->CR2, static_cast<uint32_t>(Timer::eControlRegister_2_Masks::TI1_SELECTION));
    
    // enable counter
    SetBits(mpTimer->CR1, static_cast<uint32_t>(Timer::eControlRegister_1_Masks::COUNTER_ENABLE));

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus GeneralPurposeTimer::EnableInterrupts()
{
    ASSERT(mpTimer);
    
    uint32_t updateInterruptsMask =
        static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::UPDATE_INTERRUPT);
        
    uint32_t triggerInterruptMask = static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::TRIGGER_INTERRUPT);

    // a look-up table to find appropriate mask for each channel
    const std::array<uint32_t, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> captureCompareMasks = {
        static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_1_INTERRUPT),
        static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_2_INTERRUPT),
        static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_3_INTERRUPT),
        static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_4_INTERRUPT)
    };

    // Create a mask for all interrupt bits that need to be managed
    uint32_t allInterruptsMask = updateInterruptsMask | triggerInterruptMask;

    for(const auto& mask : captureCompareMasks)
    {
        allInterruptsMask |= mask;
    }

    // TODO: replace the above loop (and the line before it) by the following
    // allInterruptsMask = std::accumulate(
    //     captureCompareMasks.begin(),
    //     captureCompareMasks.end(),
    //     updateInterruptsMask | triggerInterruptMask,  // initial value of the mask
    //     std::bit_or<>()
    // )

    // Reset all interrupt bits first
    ResetBits(mpTimer->DIER, allInterruptsMask);

    // Now set bits only which have been asked to be enabled
    uint32_t interruptsMask = 0;

    for(uint8_t i = 0; i <mpChannels.size(); i++)
    {
        auto channel = std::dynamic_pointer_cast<TimerChannel>(mpChannels[i]);
        interruptsMask |= captureCompareMasks[i];
    }

    if(mrTimerConfig.mEnableUpdateInterrupt == Timer::eUpdateInterrupt::ENABLE)
    {
        interruptsMask |= updateInterruptsMask;
    }
    
    if(mrTimerConfig.mTriggerInterrupt == Timer::eTriggerInterrupt::ENABLE)
    {
        interruptsMask |= triggerInterruptMask;
    }

    SetBits(mpTimer->DIER, interruptsMask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::EnableDma()
{
    ASSERT(mpTimer);

    // combine the required flags using bitwise OR
    uint32_t dmaMask =
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::UPDATE_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_1_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_2_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_3_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_4_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::TRIGGER_DMA_REQUEST);

    // TODO: Change based on passed configuration, hardcoded for now

    SetBits(mpTimer->DIER, dmaMask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::DisableInterrupts()
{
    ASSERT(mpTimer);

    // combine the required flags using bitwise OR
    uint32_t interruptsMask =
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::UPDATE_INTERRUPT) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_1_INTERRUPT) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_2_INTERRUPT) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_3_INTERRUPT) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_4_INTERRUPT) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::TRIGGER_INTERRUPT);

    ResetBits(mpTimer->DIER, interruptsMask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus GeneralPurposeTimer::DisableDma()
{
    ASSERT(mpTimer);

    // combine the required flags using bitwise OR
    uint32_t dmaMask =
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::UPDATE_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_1_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_2_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_3_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::CAPTURE_COMPARE_4_DMA_REQUEST) |
    static_cast<uint32_t>(Timer::eTimerDmaAndInterruptsMasks::TRIGGER_DMA_REQUEST);

    ResetBits(mpTimer->DIER, dmaMask);

    return eGeneralStatus::SUCCESS;
}

void GeneralPurposeTimer::TriggerUpdateEvent()
{
    ASSERT(mpTimer);

    uint32_t mask =
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::UPDATE_GENERATION) |
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::CAPTURE_COMPARE_1_GENERATION) |
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::CAPTURE_COMPARE_2_GENERATION) |
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::CAPTURE_COMPARE_3_GENERATION) |
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::CAPTURE_COMPARE_4_GENERATION) |
        static_cast<uint32_t>(Timer::eEventGenerationRegisterMasks::TRIGGER_GENERATION);

    SetBits(mpTimer->EGR, mask);
}

uint16_t GeneralPurposeTimer::GetStatusRegister() const
{
    ASSERT(mpTimer);

    return mpTimer->SR;
}
