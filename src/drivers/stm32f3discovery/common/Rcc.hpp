#pragma once


#include "drivers/interfaces/IRcc.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"

class RccImpl : public IRcc
{
public:
    static RccImpl* GetInstance();
    eGeneralStatus SelectSystemClock(eRccClockSource source) override;
    eGeneralStatus SwitchOnClocks(eRccClocks clocks) override;
    eGeneralStatus SwitchOffClocks(eRccClocks clocks) override;
    eGeneralStatus SelectMcoClock(eRccClocks clock) override;
    eGeneralStatus TurnRtcOnOff() override;
    eGeneralStatus SelectRtcClock(eRccClocks clock) override;
    eGeneralStatus SetUpPll(ePllMultiplicationFactor multiplication_factor);
    uint32_t GetSysClockFreq() override;
    eGeneralStatus SetAhbPrescaler(eAhbPrescaler psc) override;
    uint32_t GetAhbFrequency() override;
    eGeneralStatus SetApb1Prescaler(eApb1Apb2Prescaler psc) override;
    uint32_t GetApb1Frequency() override;
    eGeneralStatus SetApb2Prescaler(eApb1Apb2Prescaler psc) override;
    uint32_t GetApb2Frequency() override;

private:
    RCC_TypeDef *mpRCC;
    static RccImpl *mpInstance;
    static uint32_t SYS_CLK;

    RccImpl();

    RccImpl(const RccImpl&) = delete;
    RccImpl& operator=(const RccImpl&) = delete; 

};
