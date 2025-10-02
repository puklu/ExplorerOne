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
    uint32_t GetPllFreq();
    uint32_t GetSysClockFreq() override;
    eGeneralStatus SetAhbPrescaler(eAhbPrescaler psc) override;
    uint32_t GetAhbFrequency() override;
    eGeneralStatus SetApb1Prescaler(eApb1Apb2Prescaler psc) override;
    uint32_t GetApb1Frequency() override;
    eGeneralStatus SetApb2Prescaler(eApb1Apb2Prescaler psc) override;
    uint32_t GetApb2Frequency() override;

    eGeneralStatus SelectUsart1Clock(eRccClocks clock);
    eGeneralStatus SelectUsart2Clock(eRccClocks clock);
    eGeneralStatus SelectUsart3Clock(eRccClocks clock);
    eGeneralStatus SelectUart4Clock(eRccClocks clock);
    eGeneralStatus SelectUart5Clock(eRccClocks clock);
    uint32_t GetUsart1ClockFreq();
    uint32_t GetUsart2ClockFreq();
    uint32_t GetUsart3ClockFreq();
    uint32_t GetUart4ClockFreq();
    uint32_t GetUart5ClockFreq();
    eGeneralStatus SetAdcPrescaler();
    eGeneralStatus SelectAdcClock();
    uint32_t GetAdcClockFreq();
    eGeneralStatus SelectRtcClock();
    uint32_t GetRtcClockFreq();
    eGeneralStatus SelectTim1Clock(eRccClocks clock);
    uint32_t GetTim1ClockFreq();
    eGeneralStatus SelectTim8Clock(eRccClocks clock);
    uint32_t GetTim8ClockFreq();
    uint32_t GetTim15ClockFreq();
    uint32_t GetTim16ClockFreq();
    uint32_t GetTim17ClockFreq();
    uint32_t GetTim2ClockFreq();
    uint32_t GetTim3ClockFreq();
    uint32_t GetTim4ClockFreq();
    uint32_t GetTim6ClockFreq();
    uint32_t GetTim7ClockFreq();
    eGeneralStatus SelectI2c1Clock(eRccClocks clock);
    eGeneralStatus SelectI2c2Clock(eRccClocks clock);
    uint32_t GetI2c1ClockFreq();
    uint32_t GetI2c2ClockFreq();
    eGeneralStatus EnableApb1Tim2();
    eGeneralStatus EnableApb1Tim3();
    eGeneralStatus EnableApb1Tim4();
    eGeneralStatus EnableApb1Tim6();
    eGeneralStatus EnableApb1Tim7();
    eGeneralStatus EnableApb1Wwdg();
    eGeneralStatus EnableApb1Spi2();
    eGeneralStatus EnableApb1Spi3();
    eGeneralStatus EnableApb1Usart2();
    eGeneralStatus EnableApb1Usart3();
    eGeneralStatus EnableApb1Uart4();
    eGeneralStatus EnableApb1Uart5();
    eGeneralStatus EnableApb1I2c1();
    eGeneralStatus EnableApb1I2c2();
    eGeneralStatus EnableApb1Usb();
    eGeneralStatus EnableApb1Can();
    eGeneralStatus EnableApb2Tim1();
    eGeneralStatus EnableApb2Spi1();
    eGeneralStatus EnableApb2Tim8();
    eGeneralStatus EnableApb2Usart1();
    eGeneralStatus EnableApb2Tim15();
    eGeneralStatus EnableApb2Tim16();
    eGeneralStatus EnableApb2Tim17();
    eGeneralStatus EnableAhbDma1();
    eGeneralStatus EnableAhbDma2();
    eGeneralStatus EnableAhbGpioA();
    eGeneralStatus EnableAhbGpioB();
    eGeneralStatus EnableAhbGpioC();
    eGeneralStatus EnableAhbGpioD();
    eGeneralStatus EnableAhbGpioE();
    eGeneralStatus EnableAhbGpioF();
    eGeneralStatus EnableAhbAdc12();
    eGeneralStatus EnableAhbAdc34();
    
private:
    RCC_TypeDef *mpRCC;
    static RccImpl *mpInstance;
    static uint32_t SYS_CLK;
    
    RccImpl();
    
    RccImpl(const RccImpl&) = delete;
    RccImpl& operator=(const RccImpl&) = delete;
    
    uint32_t FindUsartClockSourceMask(eRccClocks clock, uint8_t usart_number);
    uint32_t GetUsartClockFreq(uint8_t usart_number);
    eGeneralStatus SelectTim_1_8_Clock(eRccClocks clock, uint8_t timer_num);
    uint32_t GetTim_1_8_15_16_17_ClockFreq(uint8_t timer_num);
    uint32_t GetTim_2_3_4_6_7_ClockFreq(uint8_t timer_num);
    eGeneralStatus SelectI2cClock(eRccClocks clock, uint8_t i2c_number);
    uint32_t GetI2cClockFreq(uint8_t i2c_number);

};
