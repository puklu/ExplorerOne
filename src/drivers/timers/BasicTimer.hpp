#pragma once

#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "common/ringBuffer.hpp"
#include "drivers/interfaces/IPin.hpp"
#include "drivers/interfaces/PinBase.hpp"

#include "ITimer.hpp"

typedef void (*InterruptCallback)(void);

struct TimerInitStruct
{
    uint16_t prescaler_value = 7999;
    uint16_t auto_reload_register_value = 1000;
    InterruptCallback cb;
    Timer::eOnePulseMode one_pulse_mode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;
    Timer::eAutoReloadPreload enable_auto_reload_preload = Timer::eAutoReloadPreload::ARR_BUFFERED;
    Timer::eUpdateRequestSource update_request_source = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;
    Timer::eUpdateEvent enable_update_event = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;
    Timer::eCounterEnable enable_counter = Timer::eCounterEnable::ENABLE;
    Timer::eMasterModeSelection master_mode_selection = Timer::eMasterModeSelection::SEND_RESET;
    Timer::eUpdateDmaRequest update_dma_request = Timer::eUpdateDmaRequest::DISABLE;
    Timer::eUpdateInterrupt enable_update_interrupt = Timer::eUpdateInterrupt::ENABLE; 
};


class BasicTimer : public ITimer
{
public:
    explicit BasicTimer(TimerInitStruct  const &timer_init_struct);
    eGeneralStatus Start() override;
    eGeneralStatus Stop() override;
    eGeneralStatus Reset() override;
    eGeneralStatus SetPeriodAndCount(uint32_t period_in_seconds, uint32_t count) override;
    eGeneralStatus EnableInterrupt() override;
    eGeneralStatus DisableInterrupt() override;
    eGeneralStatus SetPrescalerValue();
    eGeneralStatus SetAutoReloadRegisterValue();
    InterruptCallback GetInterruptCallback();
    eGeneralStatus ClearInterrupt();

    ~BasicTimer();

private:
    eGeneralStatus SetControlRegisters();
    eGeneralStatus EnableDmaAndInterrupt();
    eGeneralStatus DisableDmaAndInterrupt();
    void TriggerUpdateEvent();
    void EnableNVIC();

    RCC_TypeDef                 *mpRCC = RCC;
    TIM_TypeDef                 *mpTimer = nullptr;
    uint16_t                     mPrescalerValue;
    uint16_t                     mAutoReloadRegisterValue;
    Timer::eUpdateRequestSource  mUpdateRequestSource;
    InterruptCallback            mCallBack;
    IRQn_Type                    mIrqNumber;
    bool                         mIsInitialized = false;

};
