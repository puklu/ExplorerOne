#pragma once

#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/io/GpioPin.hpp"
#include "GeneralPurposeTimerConfig.hpp"
#include "ITimer.hpp"

typedef void (*InterruptCallback)(void);

class GeneralPurposeTimer : public ITimer
{
public:
    explicit GeneralPurposeTimer(GeneralPurposeTimerConfig  const &timer_config);
    eGeneralStatus Start() override;
    eGeneralStatus Stop() override;
    eGeneralStatus Reset() override;
    eGeneralStatus SetPeriodAndCount(uint32_t period_in_ms, uint32_t count) override;
    eGeneralStatus EnableInterrupt() override;
    eGeneralStatus DisableInterrupt() override;
    eGeneralStatus SetPrescalerValue();
    eGeneralStatus SetAutoReloadRegisterValue();
    eGeneralStatus ConfigureCaptureCompareRegisters();
    InterruptCallback GetInterruptCallback();
    ChannelConfig* GetChannels();
    eGeneralStatus ClearInterrupt();

    ~GeneralPurposeTimer();

private:
    eGeneralStatus SetControlRegisters();
    eGeneralStatus EnableDmaAndInterrupt();
    eGeneralStatus DisableDmaAndInterrupt();
    void TriggerUpdateEvent();
    void EnableNVIC();

    static eGeneralStatus ConfigureCaptureCompareSelection(volatile uint32_t *ccmr_register, Timer::eCaptureCompareSelection selection, uint8_t channel_index);
    static eGeneralStatus ConfigureInputCapturePrescaler(volatile uint32_t *ccmr_register, Timer::eInputCapturePrescaler prescaler, uint8_t channel_index);
    static eGeneralStatus ConfigureInputCaptureFilter(volatile uint32_t *ccmr_register, Timer::eInputCaptureFilter filter, uint8_t channel_index);
    static eGeneralStatus ConfigureOutputCompareMode(volatile uint32_t *ccmr_register, Timer::eOutputCompareMode mode, uint8_t channel_index);
    static eGeneralStatus ConfigureOutputComparePreloadEnable(volatile uint32_t *ccmr_register, Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index);
    eGeneralStatus EnableOutputCompare(Timer::eCaptureCompare enable, uint8_t channel_index);
    eGeneralStatus EnableInputCapture(Timer::eCaptureCompare enable, uint8_t channel_index);
    static eGeneralStatus SetAlternateFunction(ChannelConfig channel_config);
    eGeneralStatus SetPeriod(uint32_t period_in_ms);
    eGeneralStatus SetDutyCycle(volatile uint32_t *ccr_register, uint32_t period_in_ms, uint32_t duty_cycle);
    eGeneralStatus SetPeriodAndDutyCycle(volatile uint32_t *ccr_register, uint32_t period_in_ms, uint32_t duty_cycle);

    RCC_TypeDef                 *mpRCC = RCC;
    TIM_TypeDef                 *mpTimer = nullptr;
    uint16_t                     mPrescalerValue;
    uint32_t                     mAutoReloadRegisterValue;
    Timer::eUpdateRequestSource  mUpdateRequestSource;
    InterruptCallback            mCallBack;
    IRQn_Type                    mIrqNumber;
    ChannelConfig                mChannels[GENERAL_PURPOSE_TIMER_NUM_CHANNELS];
    bool                         mIsInitialized = false;

};
