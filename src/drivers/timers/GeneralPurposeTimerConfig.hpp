#pragma once

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/io/GpioPin.hpp"

typedef void (*InterruptCallback)(void);


class ChannelConfig
{
public:

    ChannelConfig() = default;

    void Init(Timer::eCaptureCompareSelection selection);
    
    GpioPin *mChannel = nullptr;
    Timer::eCaptureCompareSelection mSelection = Timer::eCaptureCompareSelection::NOT_SELECTED;

    union
    {
        struct
        {
            Timer::eInputCaptureFilter mInputCaptureFilter;
            Timer::eInputCapturePrescaler mInputCapturePrescaler;
        } mInputCaptureConfig ;
        
        struct
        {
            Timer::eOutputCompareClearEnable mOutputCompareEnable;
            Timer::eOutputComparePreloadEnable mOutputComparePreloadEnable;
            Timer::eOutputCompareMode mOutputCompareMode;
        } mOutputCompareConfig; 
    };

};


class GeneralPurposeTimerConfig
{
public:

    GeneralPurposeTimerConfig();

    ChannelConfig mChannels[GENERAL_PURPOSE_TIMER_NUM_CHANNELS]; // array of channel configurations
    
    Timer::eClockDivisionForFilter mClkDivisionForFilter = Timer::eClockDivisionForFilter::ONE;
    Timer::eAutoReloadPreload mEnableAutoReloadPreload = Timer::eAutoReloadPreload::ARR_BUFFERED;
    Timer::eDirection mDirection = Timer::eDirection::UPCOUNTER;
    Timer::eUpdateRequestSource mUpdateRequestSource = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;
    Timer::eUpdateEvent mEnableUpdateEvent = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;
    Timer::eTriggerDmaRequest mTriggerDmaRequest = Timer::eTriggerDmaRequest::DISABLE;
    Timer::eCaptureCompare mCaptureCompare4 = Timer::eCaptureCompare::DISABLE;
    Timer::eCaptureCompare mCaptureCompare3 = Timer::eCaptureCompare::DISABLE;
    Timer::eCaptureCompare mCaptureCompare2 = Timer::eCaptureCompare::DISABLE;
    Timer::eCaptureCompare mCaptureCompare1 = Timer::eCaptureCompare::DISABLE;
    Timer::eUpdateDmaRequest mUpdateDmaRequest = Timer::eUpdateDmaRequest::DISABLE;
    Timer::eTriggerInterrupt mTriggerInterrupt = Timer::eTriggerInterrupt::DISABLE;
    Timer::eCaptureCompareInterrupt mCaptureCompare4Interrupt = Timer::eCaptureCompareInterrupt::DISABLE;
    Timer::eCaptureCompareInterrupt mCaptureCompare3Interrupt = Timer::eCaptureCompareInterrupt::DISABLE;
    Timer::eCaptureCompareInterrupt mCaptureCompare2Interrupt = Timer::eCaptureCompareInterrupt::DISABLE;
    Timer::eCaptureCompareInterrupt mCaptureCompare1Interrupt = Timer::eCaptureCompareInterrupt::DISABLE;
    Timer::eUpdateInterrupt mEnableUpdateInterrupt = Timer::eUpdateInterrupt::ENABLE;


    uint16_t mPrescalerValue = 7999;
    uint32_t mAutoReloadRegisterValue = 1000;
    uint32_t mCaptureCompare4Value = 1000;
    uint32_t mCaptureCompare3Value = 1000;
    uint32_t mCaptureCompare2Value = 1000;
    uint32_t mCaptureCompare1Value = 1000;

    InterruptCallback mCb;
    Timer::eOnePulseMode mOnePulseMode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;
    Timer::eCounterEnable mEnableCounter = Timer::eCounterEnable::ENABLE;
    Timer::eMasterModeSelection mMasterModeSelection = Timer::eMasterModeSelection::SEND_RESET;
};
