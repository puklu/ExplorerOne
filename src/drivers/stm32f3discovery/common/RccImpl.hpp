/**
 * @file RccImpl.hpp
 * @brief STM32F3-specific RCC (Reset and Clock Control) implementation.
 *
 * This class provides concrete implementations for the IRcc interface for
 * STM32F3 series MCUs. It manages system clocks, peripheral clocks, PLL setup,
 * AHB/APB prescalers, and enables/disables specific peripheral clocks.
 */

#pragma once

#include "drivers/interfaces/IRcc.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"

/**
 * @class RccImpl
 * @brief Concrete implementation of the IRcc interface for STM32F3.
 *
 * This singleton class manages the STM32F3 RCC peripheral. It provides methods
 * to configure system clocks, PLL, MCO, prescalers, and peripheral clocks
 * including timers, USARTs, I2C, ADC, and GPIO.
 */
class RccImpl : public IRcc
{
   public:
    /**
     * @brief Get the singleton instance of the RCC implementation.
     * @return Pointer to the RccImpl instance.
     */
    static RccImpl* GetInstance();

    /** @brief Select the system clock source. */
    eGeneralStatus SelectSystemClock(eRccClockSource source) override;

    /** @brief Enable one or more peripheral clocks. */
    eGeneralStatus SwitchOnClocks(eRccClocks clocks) override;

    /** @brief Disable one or more peripheral clocks. */
    eGeneralStatus SwitchOffClocks(eRccClocks clocks) override;

    /** @brief Select the clock source for the Microcontroller Clock Output
     * (MCO). */
    eGeneralStatus SelectMcoClock(eRccClocks clock) override;

    /** @brief Enable or disable the RTC peripheral. */
    eGeneralStatus TurnRtcOnOff() override;

    /** @brief Select the clock source for the RTC peripheral. */
    eGeneralStatus SelectRtcClock(eRccClocks clock) override;

    /** @brief Configure the PLL multiplication factor and enable PLL. */
    eGeneralStatus SetUpPll(ePllMultiplicationFactor multiplication_factor);

    /** @brief Get the current PLL frequency. */
    uint32_t GetPllFreq();

    /** @brief Get the current system clock frequency. */
    uint32_t GetSysClockFreq() override;

    /** @brief Set the AHB prescaler. */
    eGeneralStatus SetAhbPrescaler(eAhbPrescaler psc) override;

    /** @brief Get the AHB clock frequency. */
    uint32_t GetAhbFrequency() override;

    /** @brief Set the APB1 prescaler. */
    eGeneralStatus SetApb1Prescaler(eApb1Apb2Prescaler psc) override;

    /** @brief Get the APB1 clock frequency. */
    uint32_t GetApb1Frequency() override;

    /** @brief Set the APB2 prescaler. */
    eGeneralStatus SetApb2Prescaler(eApb1Apb2Prescaler psc) override;

    /** @brief Get the APB2 clock frequency. */
    uint32_t GetApb2Frequency() override;

    /** @brief Peripheral-specific clock selection and frequency getters */
    eGeneralStatus SelectUsart1Clock(eRccClocks clock);
    eGeneralStatus SelectUsart2Clock(eRccClocks clock);
    eGeneralStatus SelectUsart3Clock(eRccClocks clock);
    eGeneralStatus SelectUart4Clock(eRccClocks clock);
    eGeneralStatus SelectUart5Clock(eRccClocks clock);
    uint32_t       GetUsart1ClockFreq();
    uint32_t       GetUsart2ClockFreq();
    uint32_t       GetUsart3ClockFreq();
    uint32_t       GetUart4ClockFreq();
    uint32_t       GetUart5ClockFreq();
    eGeneralStatus SetAdcPrescaler();
    eGeneralStatus SelectAdcClock();
    uint32_t       GetAdcClockFreq();
    eGeneralStatus SelectRtcClock();
    uint32_t       GetRtcClockFreq();
    eGeneralStatus SelectTim1Clock(eRccClocks clock);
    uint32_t       GetTim1ClockFreq();
    eGeneralStatus SelectTim8Clock(eRccClocks clock);
    uint32_t       GetTim8ClockFreq();
    uint32_t       GetTim15ClockFreq();
    uint32_t       GetTim16ClockFreq();
    uint32_t       GetTim17ClockFreq();
    uint32_t       GetTim2ClockFreq();
    uint32_t       GetTim3ClockFreq();
    uint32_t       GetTim4ClockFreq();
    uint32_t       GetTim6ClockFreq();
    uint32_t       GetTim7ClockFreq();
    eGeneralStatus SelectI2c1Clock(eRccClocks clock);
    eGeneralStatus SelectI2c2Clock(eRccClocks clock);
    uint32_t       GetI2c1ClockFreq();
    uint32_t       GetI2c2ClockFreq();

    /** @brief Peripheral clock enable functions */
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
    /** @brief Pointer to the STM32 RCC peripheral registers */
    RCC_TypeDef* mpRCC;

    /** @brief Private constructor for singleton pattern */
    RccImpl();

    /** @brief Deleted copy constructor */
    RccImpl(const RccImpl&) = delete;

    /** @brief Deleted assignment operator */
    RccImpl& operator=(const RccImpl&) = delete;

    /** @brief Helper functions for peripheral clock handling */
    uint32_t FindUsartClockSourceMask(eRccClocks clock, uint8_t usart_number);
    uint32_t GetUsartClockFreq(uint8_t usart_number);
    eGeneralStatus SelectTim_1_8_Clock(eRccClocks clock, uint8_t timer_num);
    uint32_t       GetTim_1_8_15_16_17_ClockFreq(uint8_t timer_num);
    uint32_t       GetTim_2_3_4_6_7_ClockFreq(uint8_t timer_num);
    eGeneralStatus SelectI2cClock(eRccClocks clock, uint8_t i2c_number);
    uint32_t       GetI2cClockFreq(uint8_t i2c_number);

    /**
     * @brief  Configure Flash latency and prefetch for high-speed operation.
     * @note   Must be called BEFORE switching to a higher clock (e.g., enabling
     * PLL)
     */
    eGeneralStatus ConfigureFlashLatency();
};
