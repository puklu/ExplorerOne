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
    eGeneralStatus SetPeriodAndCount(uint32_t period_in_seconds, uint32_t count) override;
    eGeneralStatus EnableInterrupt() override;
    eGeneralStatus DisableInterrupt() override;
    eGeneralStatus SetPrescalerValue();
    eGeneralStatus SetAutoReloadRegisterValue();
    eGeneralStatus ConfigureCaptureCompareRegisters(GeneralPurposeTimerConfig const &timer_config);
    InterruptCallback GetInterruptCallback();
    eGeneralStatus ClearInterrupt();

    ~GeneralPurposeTimer();

private:
    eGeneralStatus SetControlRegisters();
    eGeneralStatus EnableDmaAndInterrupt();
    eGeneralStatus DisableDmaAndInterrupt();
    void TriggerUpdateEvent();
    void EnableNVIC();

    static eGeneralStatus ConfigureInputCapturePrescaler(volatile uint32_t *ccmr_register, Timer::eInputCapturePrescaler prescaler, uint8_t channel_index);
    static eGeneralStatus ConfigureInputCaptureFilter(volatile uint32_t *ccmr_register, Timer::eInputCaptureFilter filter, uint8_t channel_index);
    static eGeneralStatus ConfigureOutputCompareMode(volatile uint32_t *ccmr_register, Timer::eOutputCompareMode mode, uint8_t channel_index);
    static eGeneralStatus ConfigureOutputComparePreloadEnable(volatile uint32_t *ccmr_register, Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index);
    eGeneralStatus EnableOutputCompare(Timer::eCaptureCompare enable, uint8_t channel_index);
    eGeneralStatus EnableInputCapture(Timer::eCaptureCompare enable, uint8_t channel_index);

    RCC_TypeDef                 *mpRCC = RCC;
    TIM_TypeDef                 *mpTimer = nullptr;
    uint16_t                     mPrescalerValue;
    uint32_t                     mAutoReloadRegisterValue;
    Timer::eUpdateRequestSource  mUpdateRequestSource;
    InterruptCallback            mCallBack;
    IRQn_Type                    mIrqNumber;
    GpioPin                     *mChannel1 = nullptr;
    GpioPin                     *mChannel2 = nullptr;
    GpioPin                     *mChannel3 = nullptr;
    GpioPin                     *mChannel4 = nullptr;
    bool                         mIsInitialized = false;

};
