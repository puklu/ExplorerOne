/**
 * @file File for all arrays used to hold address and masks
 * defined in header file provided by stm so that the arrays
 * can be conveniently used by the interface without having to
 * deal with crude registers.
 */

#pragma once

#include "common/defines.hpp"
#include "stm32f303xc.h"

namespace IO
{

inline GPIO_TypeDef *const aPorts[IO_PORT_COUNT] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF,
};

inline long unsigned int const aPortEnableRegisters[IO_PORT_COUNT] = {
    RCC_AHBENR_GPIOAEN, RCC_AHBENR_GPIOBEN, RCC_AHBENR_GPIOCEN,
    RCC_AHBENR_GPIODEN, RCC_AHBENR_GPIOEEN, RCC_AHBENR_GPIOFEN};

inline long unsigned int const aModeRegisterBits[IO_BITS_PER_REGISTER] = {
    GPIO_MODER_MODER0_0,  GPIO_MODER_MODER0_1,  GPIO_MODER_MODER1_0,
    GPIO_MODER_MODER1_1,  GPIO_MODER_MODER2_0,  GPIO_MODER_MODER2_1,
    GPIO_MODER_MODER3_0,  GPIO_MODER_MODER3_1,  GPIO_MODER_MODER4_0,
    GPIO_MODER_MODER4_1,  GPIO_MODER_MODER5_0,  GPIO_MODER_MODER5_1,
    GPIO_MODER_MODER6_0,  GPIO_MODER_MODER6_1,  GPIO_MODER_MODER7_0,
    GPIO_MODER_MODER7_1,  GPIO_MODER_MODER8_0,  GPIO_MODER_MODER8_1,
    GPIO_MODER_MODER9_0,  GPIO_MODER_MODER9_1,  GPIO_MODER_MODER10_0,
    GPIO_MODER_MODER10_1, GPIO_MODER_MODER11_0, GPIO_MODER_MODER11_1,
    GPIO_MODER_MODER12_0, GPIO_MODER_MODER12_1, GPIO_MODER_MODER13_0,
    GPIO_MODER_MODER13_1, GPIO_MODER_MODER14_0, GPIO_MODER_MODER14_1,
    GPIO_MODER_MODER15_0, GPIO_MODER_MODER15_1,
};

inline long unsigned int const aOutputTypeRegisterBits[IO_PIN_COUNT_PER_PORT] =
    {GPIO_OTYPER_OT_0,  GPIO_OTYPER_OT_1,  GPIO_OTYPER_OT_2,
     GPIO_OTYPER_OT_3,  GPIO_OTYPER_OT_4,  GPIO_OTYPER_OT_5,
     GPIO_OTYPER_OT_6,  GPIO_OTYPER_OT_7,  GPIO_OTYPER_OT_8,
     GPIO_OTYPER_OT_9,  GPIO_OTYPER_OT_10, GPIO_OTYPER_OT_11,
     GPIO_OTYPER_OT_12, GPIO_OTYPER_OT_13, GPIO_OTYPER_OT_14,
     GPIO_OTYPER_OT_15};

inline long unsigned int const aOutputSpeedRegisterBits[IO_BITS_PER_REGISTER] =
    {
        GPIO_OSPEEDER_OSPEEDR0_0,  GPIO_OSPEEDER_OSPEEDR0_1,
        GPIO_OSPEEDER_OSPEEDR1_0,  GPIO_OSPEEDER_OSPEEDR1_1,
        GPIO_OSPEEDER_OSPEEDR2_0,  GPIO_OSPEEDER_OSPEEDR2_1,
        GPIO_OSPEEDER_OSPEEDR3_0,  GPIO_OSPEEDER_OSPEEDR3_1,
        GPIO_OSPEEDER_OSPEEDR4_0,  GPIO_OSPEEDER_OSPEEDR4_1,
        GPIO_OSPEEDER_OSPEEDR5_0,  GPIO_OSPEEDER_OSPEEDR5_1,
        GPIO_OSPEEDER_OSPEEDR6_0,  GPIO_OSPEEDER_OSPEEDR6_1,
        GPIO_OSPEEDER_OSPEEDR7_0,  GPIO_OSPEEDER_OSPEEDR7_1,
        GPIO_OSPEEDER_OSPEEDR8_0,  GPIO_OSPEEDER_OSPEEDR8_1,
        GPIO_OSPEEDER_OSPEEDR9_0,  GPIO_OSPEEDER_OSPEEDR9_1,
        GPIO_OSPEEDER_OSPEEDR10_0, GPIO_OSPEEDER_OSPEEDR10_1,
        GPIO_OSPEEDER_OSPEEDR11_0, GPIO_OSPEEDER_OSPEEDR11_1,
        GPIO_OSPEEDER_OSPEEDR12_0, GPIO_OSPEEDER_OSPEEDR12_1,
        GPIO_OSPEEDER_OSPEEDR13_0, GPIO_OSPEEDER_OSPEEDR13_1,
        GPIO_OSPEEDER_OSPEEDR14_0, GPIO_OSPEEDER_OSPEEDR14_1,
        GPIO_OSPEEDER_OSPEEDR15_0, GPIO_OSPEEDER_OSPEEDR15_1,
};

inline long unsigned int const
    aPullupPulldownRegisterBits[IO_BITS_PER_REGISTER] = {
        GPIO_PUPDR_PUPDR0_0,  GPIO_PUPDR_PUPDR0_1,  GPIO_PUPDR_PUPDR1_0,
        GPIO_PUPDR_PUPDR1_1,  GPIO_PUPDR_PUPDR2_0,  GPIO_PUPDR_PUPDR2_1,
        GPIO_PUPDR_PUPDR3_0,  GPIO_PUPDR_PUPDR3_1,  GPIO_PUPDR_PUPDR4_0,
        GPIO_PUPDR_PUPDR4_1,  GPIO_PUPDR_PUPDR5_0,  GPIO_PUPDR_PUPDR5_1,
        GPIO_PUPDR_PUPDR6_0,  GPIO_PUPDR_PUPDR6_1,  GPIO_PUPDR_PUPDR7_0,
        GPIO_PUPDR_PUPDR7_1,  GPIO_PUPDR_PUPDR8_0,  GPIO_PUPDR_PUPDR8_1,
        GPIO_PUPDR_PUPDR9_0,  GPIO_PUPDR_PUPDR9_1,  GPIO_PUPDR_PUPDR10_0,
        GPIO_PUPDR_PUPDR10_1, GPIO_PUPDR_PUPDR11_0, GPIO_PUPDR_PUPDR11_1,
        GPIO_PUPDR_PUPDR12_0, GPIO_PUPDR_PUPDR12_1, GPIO_PUPDR_PUPDR13_0,
        GPIO_PUPDR_PUPDR13_1, GPIO_PUPDR_PUPDR14_0, GPIO_PUPDR_PUPDR14_1,
        GPIO_PUPDR_PUPDR15_0, GPIO_PUPDR_PUPDR15_1,
};

inline long unsigned int const aInputDataRegisterBits[IO_PIN_COUNT_PER_PORT] = {
    GPIO_IDR_0,  GPIO_IDR_1,  GPIO_IDR_2,  GPIO_IDR_3,
    GPIO_IDR_4,  GPIO_IDR_5,  GPIO_IDR_6,  GPIO_IDR_7,
    GPIO_IDR_8,  GPIO_IDR_9,  GPIO_IDR_10, GPIO_IDR_11,
    GPIO_IDR_12, GPIO_IDR_13, GPIO_IDR_14, GPIO_IDR_15,
};

inline long unsigned int const aOutputDataRegisterBits[IO_PIN_COUNT_PER_PORT] =
    {
        GPIO_ODR_0,  GPIO_ODR_1,  GPIO_ODR_2,  GPIO_ODR_3,
        GPIO_ODR_4,  GPIO_ODR_5,  GPIO_ODR_6,  GPIO_ODR_7,
        GPIO_ODR_8,  GPIO_ODR_9,  GPIO_ODR_10, GPIO_ODR_11,
        GPIO_ODR_12, GPIO_ODR_13, GPIO_ODR_14, GPIO_ODR_15,
};

inline long unsigned int const aExtiImrRegisterBits[IO_PIN_COUNT_PER_PORT] =
    {
        // Using only 16 lines for now, each EXTI corresponds to each pin number
        // for eg: EXTI0 corresponds to pin 0 of each port. Which port it belongs
        // to is set in SYSCFG
        EXTI_IMR_MR0,
        EXTI_IMR_MR1,
        EXTI_IMR_MR2,
        EXTI_IMR_MR3,
        EXTI_IMR_MR4,
        EXTI_IMR_MR5,
        EXTI_IMR_MR6,
        EXTI_IMR_MR7,
        EXTI_IMR_MR8,
        EXTI_IMR_MR9,
        EXTI_IMR_MR10,
        EXTI_IMR_MR11,
        EXTI_IMR_MR12,
        EXTI_IMR_MR13,
        EXTI_IMR_MR14,
        EXTI_IMR_MR15,
        // EXTI_IMR_MR16,
        // EXTI_IMR_MR17,
        // EXTI_IMR_MR18,
        // EXTI_IMR_MR19,
        // EXTI_IMR_MR20,
        // EXTI_IMR_MR20,
        // EXTI_IMR_MR21,
        // EXTI_IMR_MR22,
        // EXTI_IMR_MR23,
        // EXTI_IMR_MR24,
        // EXTI_IMR_MR25,
        // EXTI_IMR_MR26,
        // EXTI_IMR_MR27,
        // EXTI_IMR_MR28,
        // EXTI_IMR_MR29,
        // EXTI_IMR_MR30,   
        // EXTI_IMR_MR31,
        // EXTI_IMR2_MR32,
        // EXTI_IMR2_MR33,
        // EXTI_IMR2_MR34,
        // EXTI_IMR2_MR35,
};

inline IRQn_Type aExtiIrqNumbers[IO_PIN_COUNT_PER_PORT] = {
    EXTI0_IRQn, EXTI1_IRQn, EXTI2_TSC_IRQn, EXTI3_IRQn, EXTI4_IRQn,
    // EXTI5 TO EXTI9 share an IRQ
    EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,
    // EXTI15 TO EXTI15 share an IRQ
    EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn,
    EXTI15_10_IRQn, EXTI15_10_IRQn
};


// inline long unsigned int const aExtiRisingEdgeTriggerRegisterBits[IO_EXTI_LINES_COUNT] =
//     {   
//         EXTI_RTSR_TR0, EXTI_RTSR_TR1 , EXTI_RTSR_TR2, EXTI_RTSR_TR3, EXTI_RTSR_TR4,
//         EXTI_RTSR_TR5, EXTI_RTSR_TR6 , EXTI_RTSR_TR7, EXTI_RTSR_TR8, EXTI_RTSR_TR9,
//         EXTI_RTSR_TR10, EXTI_RTSR_TR11 , EXTI_RTSR_TR12, EXTI_RTSR_TR13, EXTI_RTSR_TR14,
//         EXTI_RTSR_TR11, EXTI_RTSR_TR12 , EXTI_RTSR_TR13, EXTI_RTSR_TR14, EXTI_RTSR_TR15,
//         EXTI_RTSR_TR16, EXTI_RTSR_TR17 , EXTI_RTSR_TR18, EXTI_RTSR_TR19, EXTI_RTSR_TR20,
//         EXTI_RTSR_TR21, EXTI_RTSR_TR22 , EXTI_RTSR_TR29, EXTI_RTSR_TR30,
//         EXTI_RTSR_TR31, EXTI_RTSR2_TR32, EXTI_RTSR2_TR33, 
//     };
  

inline long unsigned int const aSyscfgExtiRegisterBits[IO_EXTI_LINES_COUNT]
    {
        SYSCFG_EXTICR1_EXTI0,
        SYSCFG_EXTICR1_EXTI1,
        SYSCFG_EXTICR1_EXTI2,
        SYSCFG_EXTICR1_EXTI3,
        SYSCFG_EXTICR2_EXTI4,
        SYSCFG_EXTICR2_EXTI5,
        SYSCFG_EXTICR2_EXTI6,
        SYSCFG_EXTICR2_EXTI7,
        SYSCFG_EXTICR3_EXTI8,
        SYSCFG_EXTICR3_EXTI9,
        SYSCFG_EXTICR3_EXTI10,
        SYSCFG_EXTICR3_EXTI11,
        SYSCFG_EXTICR4_EXTI12,
        SYSCFG_EXTICR4_EXTI13,
        SYSCFG_EXTICR4_EXTI14,
        SYSCFG_EXTICR4_EXTI15,
    };

inline long unsigned int const aUsartEnableRegistersMasks[COUNT_OF_USARTS] = {
    RCC_APB2ENR_USART1EN, RCC_APB1ENR_USART2EN, RCC_APB1ENR_USART3EN,
    RCC_APB1ENR_UART4EN, RCC_APB1ENR_UART5EN};


}  // namespace IO

inline TIM_TypeDef* const aBasicTimersAddress[NUMBER_OF_BASIC_TIMERS] = {
    TIM6, TIM7};  

inline long unsigned int const aBasicTimersEnableMasks[NUMBER_OF_BASIC_TIMERS] = {
    RCC_APB1ENR_TIM6EN, RCC_APB1ENR_TIM7EN};

inline IRQn_Type aBasicTimersIrqNumbers[NUMBER_OF_BASIC_TIMERS] = {
   TIM6_DAC_IRQn, TIM7_IRQn};


inline TIM_TypeDef* const aGeneralPurposeTimersAddress[NUMBER_OF_GENERAL_PURPOSE_TIMERS] = {
    TIM2, TIM3, TIM4};

inline long unsigned int const aGeneralPurposeTimersEnableMasks[NUMBER_OF_GENERAL_PURPOSE_TIMERS] = {
    RCC_APB1ENR_TIM2EN, RCC_APB1ENR_TIM3EN, RCC_APB1ENR_TIM4EN};    

inline IRQn_Type aGeneralPurposeTimersIrqNumbers[NUMBER_OF_GENERAL_PURPOSE_TIMERS] = {
   TIM2_IRQn , TIM3_IRQn, TIM4_IRQn};

inline long unsigned int const aGeneralPurposeTimerCcerRegisterMasks[GENERAL_PURPOSE_TIMER_NUM_CHANNELS][3] = {
    {TIM_CCER_CC1E, TIM_CCER_CC1P, TIM_CCER_CC1NP},
    {TIM_CCER_CC2E, TIM_CCER_CC2P, TIM_CCER_CC2NP},
    {TIM_CCER_CC3E, TIM_CCER_CC3P, TIM_CCER_CC3NP},
    {TIM_CCER_CC4E, TIM_CCER_CC4P, TIM_CCER_CC4NP},
};

inline long unsigned int const aGeneralPurposeTimerCcmrOutputCompareRegisterMasks[GENERAL_PURPOSE_TIMER_NUM_CHANNELS][11] = {
    {TIM_CCMR1_CC1S, TIM_CCMR1_CC1S_0, TIM_CCMR1_CC1S_1, TIM_CCMR1_OC1FE, TIM_CCMR1_OC1PE, TIM_CCMR1_OC1M, TIM_CCMR1_OC1M_0, TIM_CCMR1_OC1M_1, TIM_CCMR1_OC1M_2, TIM_CCMR1_OC1M_3, TIM_CCMR1_OC1CE},
    {TIM_CCMR1_CC2S, TIM_CCMR1_CC2S_0, TIM_CCMR1_CC2S_1, TIM_CCMR1_OC2FE, TIM_CCMR1_OC2PE, TIM_CCMR1_OC2M, TIM_CCMR1_OC2M_0, TIM_CCMR1_OC2M_1, TIM_CCMR1_OC2M_2, TIM_CCMR1_OC2M_3, TIM_CCMR1_OC2CE},
    {TIM_CCMR2_CC3S, TIM_CCMR2_CC3S_0, TIM_CCMR2_CC3S_1, TIM_CCMR2_OC3FE, TIM_CCMR2_OC3PE, TIM_CCMR2_OC3M, TIM_CCMR2_OC3M_0, TIM_CCMR2_OC3M_1, TIM_CCMR2_OC3M_2, TIM_CCMR2_OC3M_3, TIM_CCMR2_OC3CE},
    {TIM_CCMR2_CC4S, TIM_CCMR2_CC4S_0, TIM_CCMR2_CC4S_1, TIM_CCMR2_OC4FE, TIM_CCMR2_OC4PE, TIM_CCMR2_OC4M, TIM_CCMR2_OC4M_0, TIM_CCMR2_OC4M_1, TIM_CCMR2_OC4M_2, TIM_CCMR2_OC4M_3, TIM_CCMR2_OC4CE},
};

inline long unsigned int const aGeneralPurposeTimerCcmrInputCaptureRegisterMasks[GENERAL_PURPOSE_TIMER_NUM_CHANNELS][11] = {
    {TIM_CCMR1_CC1S, TIM_CCMR1_CC1S_0, TIM_CCMR1_CC1S_1, TIM_CCMR1_IC1PSC, TIM_CCMR1_IC1PSC_0, TIM_CCMR1_IC1PSC_1, TIM_CCMR1_IC1F, TIM_CCMR1_IC1F_0, TIM_CCMR1_IC1F_1, TIM_CCMR1_IC1F_2, TIM_CCMR1_IC1F_3},
    {TIM_CCMR1_CC2S, TIM_CCMR1_CC2S_0, TIM_CCMR1_CC2S_1, TIM_CCMR1_IC2PSC, TIM_CCMR1_IC2PSC_0, TIM_CCMR1_IC2PSC_1, TIM_CCMR1_IC2F, TIM_CCMR1_IC2F_0, TIM_CCMR1_IC2F_1, TIM_CCMR1_IC2F_2, TIM_CCMR1_IC2F_3},
    {TIM_CCMR2_CC3S, TIM_CCMR2_CC3S_0, TIM_CCMR2_CC3S_1, TIM_CCMR2_IC3PSC, TIM_CCMR2_IC3PSC_0, TIM_CCMR2_IC3PSC_1, TIM_CCMR2_IC3F, TIM_CCMR2_IC3F_0, TIM_CCMR2_IC3F_1, TIM_CCMR2_IC3F_2, TIM_CCMR2_IC3F_3},
    {TIM_CCMR2_CC4S, TIM_CCMR2_CC4S_0, TIM_CCMR2_CC4S_1, TIM_CCMR2_IC4PSC, TIM_CCMR2_IC4PSC_0, TIM_CCMR2_IC4PSC_1, TIM_CCMR2_IC4F, TIM_CCMR2_IC4F_0, TIM_CCMR2_IC4F_1, TIM_CCMR2_IC4F_2, TIM_CCMR2_IC4F_3},
};

struct aSysTick
{
    // Address of the systick peripheral
    inline static SysTick_Type * ADDRESS = SysTick; 

    // control and status register masks
    struct CTRL
    {
        static constexpr uint32_t CTRL_COUNTFLAG = SysTick_CTRL_COUNTFLAG_Msk;
        static constexpr uint32_t CTRL_CLKSOURCE = SysTick_CTRL_CLKSOURCE_Msk;
        static constexpr uint32_t CTRL_TICKINT = SysTick_CTRL_TICKINT_Msk;
        static constexpr uint32_t CTRL_ENABLE = SysTick_CTRL_ENABLE_Msk;
    };

    // load and value register masks
    struct VAL
    {
        static constexpr uint32_t LOAD_RELOAD = SysTick_LOAD_RELOAD_Msk;
        static constexpr uint32_t VAL_CURRENT = SysTick_VAL_CURRENT_Msk;
    };

    // calibration register masks
    struct CALIB
    {
        static constexpr uint32_t CALIB_NOREF = SysTick_CALIB_NOREF_Msk;
        static constexpr uint32_t CALIB_SKEW = SysTick_CALIB_SKEW_Msk;
        static constexpr uint32_t CALIB_TENMS = SysTick_CALIB_TENMS_Msk;
    };
};

struct aRcc
{
    inline static RCC_TypeDef* ADDRESS = RCC;    
    struct RCC_CR
    {
        static constexpr uint32_t HSI_ON = RCC_CR_HSION;
        static constexpr uint32_t HSI_READY = RCC_CR_HSIRDY;
        static constexpr uint32_t PLL_ON = RCC_CR_PLLON;
        static constexpr uint32_t PLL_READY = RCC_CR_PLLRDY;
    };

    struct RCC_CFGR
    {
        static constexpr uint32_t SYSTEM_CLOCK_SWITCH = RCC_CFGR_SW;
        struct CLOCK_SELECTION_MASK
        {
            static constexpr uint32_t HSI = RCC_CFGR_SW_HSI;
            static constexpr uint32_t HSE = RCC_CFGR_SW_HSE;
            static constexpr uint32_t PLL = RCC_CFGR_SW_PLL;
        };

        static constexpr uint32_t SYSTEM_CLOCK_SWITCH_STATUS = RCC_CFGR_SWS;
        struct SYSTEM_CLOCK_SWITCH_STATUS_MASK
        {
            static constexpr uint32_t HSI = RCC_CFGR_SWS_HSI;
            static constexpr uint32_t HSE = RCC_CFGR_SWS_HSE;
            static constexpr uint32_t PLL = RCC_CFGR_SWS_PLL;
        };

        static constexpr uint32_t AHB_PRESCALER = RCC_CFGR_HPRE;
        static constexpr uint32_t AHB_PRESCALER_POSITION = RCC_CFGR_HPRE_Pos; 
        struct AHB_PRESCALER_MASK
        {
            static constexpr uint32_t DIVIDE_BY_1 = RCC_CFGR_HPRE_DIV1;
            static constexpr uint32_t DIVIDE_BY_2 = RCC_CFGR_HPRE_DIV2;
            static constexpr uint32_t DIVIDE_BY_4 = RCC_CFGR_HPRE_DIV4;
            static constexpr uint32_t DIVIDE_BY_8 = RCC_CFGR_HPRE_DIV8;
            static constexpr uint32_t DIVIDE_BY_16 = RCC_CFGR_HPRE_DIV16;
            static constexpr uint32_t DIVIDE_BY_64 = RCC_CFGR_HPRE_DIV64;
            static constexpr uint32_t DIVIDE_BY_128 = RCC_CFGR_HPRE_DIV128;
            static constexpr uint32_t DIVIDE_BY_256 = RCC_CFGR_HPRE_DIV256;
            static constexpr uint32_t DIVIDE_BY_512 = RCC_CFGR_HPRE_DIV512;
        };

        static constexpr uint32_t APB1_PRESCALER = RCC_CFGR_PPRE1;
        static constexpr uint32_t APB1_PRESCALER_POSITION = RCC_CFGR_PPRE1_Pos;
        struct APB1_PRESCALER_MASK
        {
            static constexpr uint32_t DIVIDE_BY_1 = RCC_CFGR_PPRE1_DIV1;
            static constexpr uint32_t DIVIDE_BY_2 = RCC_CFGR_PPRE1_DIV2;
            static constexpr uint32_t DIVIDE_BY_4 = RCC_CFGR_PPRE1_DIV4;
            static constexpr uint32_t DIVIDE_BY_8 = RCC_CFGR_PPRE1_DIV8;
            static constexpr uint32_t DIVIDE_BY_16 = RCC_CFGR_PPRE1_DIV16;
        };

        static constexpr uint32_t APB2_PRESCALER = RCC_CFGR_PPRE2;
        static constexpr uint32_t APB2_PRESCALER_POSITION = RCC_CFGR_PPRE2_Pos;
        struct APB2_PRESCALER_MASK
        {
            static constexpr uint32_t DIVIDE_BY_1 = RCC_CFGR_PPRE2_DIV1;
            static constexpr uint32_t DIVIDE_BY_2 = RCC_CFGR_PPRE2_DIV2;
            static constexpr uint32_t DIVIDE_BY_4 = RCC_CFGR_PPRE2_DIV4;
            static constexpr uint32_t DIVIDE_BY_8 = RCC_CFGR_PPRE2_DIV8;
            static constexpr uint32_t DIVIDE_BY_16 = RCC_CFGR_PPRE2_DIV16;
        };

        static constexpr uint32_t PLL_CLOCK_SOURCE = RCC_CFGR_PLLSRC;
        struct PLL_CLOCK_SOURCE_MASK
        {
            static constexpr uint32_t HSI_DIVIDED_BY_2 = RCC_CFGR_PLLSRC_HSI_DIV2;
            static constexpr uint32_t HSE_DIVIDED_BY_PREDIV = RCC_CFGR_PLLSRC_HSE_PREDIV;
        };

        static constexpr uint32_t PLL_MULTIPLICATION_FACTOR = RCC_CFGR_PLLMUL;
        static constexpr uint32_t PLL_MULTIPLICATION_FACTOR_POSITION = RCC_CFGR_PLLMUL_Pos;
        struct PLL_MULTIPLICATION_FACTOR_MASK
        {
            static constexpr uint32_t INPUT_CLOCK_X2 = RCC_CFGR_PLLMUL2;
            static constexpr uint32_t INPUT_CLOCK_X3 = RCC_CFGR_PLLMUL3;
            static constexpr uint32_t INPUT_CLOCK_X4 = RCC_CFGR_PLLMUL4;
            static constexpr uint32_t INPUT_CLOCK_X5 = RCC_CFGR_PLLMUL5;
            static constexpr uint32_t INPUT_CLOCK_X6 = RCC_CFGR_PLLMUL6;
            static constexpr uint32_t INPUT_CLOCK_X7 = RCC_CFGR_PLLMUL7;
            static constexpr uint32_t INPUT_CLOCK_X8 = RCC_CFGR_PLLMUL8;
            static constexpr uint32_t INPUT_CLOCK_X9 = RCC_CFGR_PLLMUL9;
            static constexpr uint32_t INPUT_CLOCK_X10 = RCC_CFGR_PLLMUL10;
            static constexpr uint32_t INPUT_CLOCK_X11 = RCC_CFGR_PLLMUL11;
            static constexpr uint32_t INPUT_CLOCK_X12 = RCC_CFGR_PLLMUL12;
            static constexpr uint32_t INPUT_CLOCK_X13 = RCC_CFGR_PLLMUL13;
            static constexpr uint32_t INPUT_CLOCK_X14 = RCC_CFGR_PLLMUL14;
            static constexpr uint32_t INPUT_CLOCK_X15 = RCC_CFGR_PLLMUL15;
            static constexpr uint32_t INPUT_CLOCK_X16 = RCC_CFGR_PLLMUL16;
        };

        static constexpr uint32_t MICRCONTROLLER_CLOCK_OUTPUT = RCC_CFGR_MCO;
        struct MICRCONTROLLER_CLOCK_OUTPUT_MASK
        {
            static constexpr uint32_t NO_CLOCK_ON_MCO = RCC_CFGR_MCO_NOCLOCK;
            static constexpr uint32_t LSI_ON_MCO = RCC_CFGR_MCO_LSI;
            static constexpr uint32_t LSE_ON_MCO = RCC_CFGR_MCO_LSE;
            static constexpr uint32_t SYSCLK_ON_MCO = RCC_CFGR_MCO_SYSCLK;
            static constexpr uint32_t HSI_ON_MCO = RCC_CFGR_MCO_HSI;
            static constexpr uint32_t HSE_ON_MCO = RCC_CFGR_MCO_HSE;
            static constexpr uint32_t PLL_ON_MCO = RCC_CFGR_MCO_PLL;
        };
    };

    struct RCC_CFGR3
    {
        static constexpr uint32_t USART1_SOURCE_SELECTION = RCC_CFGR3_USART1SW;
        static constexpr uint32_t USART1_SOURCE_SELECTION_POSITION = RCC_CFGR3_USART1SW_Pos;
        struct USART1_SOURCES_MASK
        {
            static constexpr uint32_t PCLK2 = RCC_CFGR3_USART1SW_PCLK2;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_USART1SW_SYSCLK;
            static constexpr uint32_t LSE = RCC_CFGR3_USART1SW_LSE;
            static constexpr uint32_t HSI = RCC_CFGR3_USART1SW_HSI;
        };
        static constexpr uint32_t I2C1_SOURCE_SELECTION = RCC_CFGR3_I2C1SW;
        struct I2C1_SOURCES_MASK
        {
            static constexpr uint32_t HSI = RCC_CFGR3_I2C1SW_HSI;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_I2C1SW_SYSCLK;
        };

        static constexpr uint32_t I2C2_SOURCE_SELECTION = RCC_CFGR3_I2C2SW;
        struct I2C2_SOURCES_MASK
        {
            static constexpr uint32_t HSI = RCC_CFGR3_I2C2SW_HSI;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_I2C2SW_SYSCLK;
        };

        static constexpr uint32_t TIM1_SOURCE_SELECTION = RCC_CFGR3_TIM1SW;
        static constexpr uint32_t TIM1_SOURCE_SELECTION_POSITION = RCC_CFGR3_TIM1SW_Pos;
        struct TIM1_SOURCES_MASK
        {
            static constexpr uint32_t PCLK2 = RCC_CFGR3_TIM1SW_PCLK2;
            static constexpr uint32_t PLL = RCC_CFGR3_TIM1SW_PLL;
        };

        static constexpr uint32_t TIM8_SOURCE_SELECTION = RCC_CFGR3_TIM8SW;
        static constexpr uint32_t TIM8_SOURCE_SELECTION_POSITION = RCC_CFGR3_TIM8SW_Pos;
        struct TIM8_SOURCES_MASK
        {
            static constexpr uint32_t PCLK2 = RCC_CFGR3_TIM8SW_PCLK2;
            static constexpr uint32_t PLL = RCC_CFGR3_TIM8SW_PLL;
        };
        
        static constexpr uint32_t USART2_SOURCE_SELECTION = RCC_CFGR3_USART2SW;
        static constexpr uint32_t USART2_SOURCE_SELECTION_POSITION = RCC_CFGR3_USART2SW_Pos;
        struct USART2_SOURCES_MASK
        {
            static constexpr uint32_t PCLK = RCC_CFGR3_USART2SW_PCLK;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_USART2SW_SYSCLK;
            static constexpr uint32_t LSE = RCC_CFGR3_USART2SW_LSE;
            static constexpr uint32_t HSI = RCC_CFGR3_USART2SW_HSI;
        };

        static constexpr uint32_t USART3_SOURCE_SELECTION = RCC_CFGR3_USART3SW;
        static constexpr uint32_t USART3_SOURCE_SELECTION_POSITION = RCC_CFGR3_USART3SW_Pos;
        struct USART3_SOURCES_MASK
        {
            static constexpr uint32_t PCLK = RCC_CFGR3_USART3SW_PCLK;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_USART3SW_SYSCLK;
            static constexpr uint32_t LSE = RCC_CFGR3_USART3SW_LSE;
            static constexpr uint32_t HSI = RCC_CFGR3_USART3SW_HSI;
        };

        static constexpr uint32_t UART4_SOURCE_SELECTION = RCC_CFGR3_UART4SW;
        static constexpr uint32_t UART4_SOURCE_SELECTION_POSITION = RCC_CFGR3_UART4SW_Pos;
        struct UART4_SOURCES_MASK
        {
            static constexpr uint32_t PCLK = RCC_CFGR3_UART4SW_PCLK;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_UART4SW_SYSCLK;
            static constexpr uint32_t LSE = RCC_CFGR3_UART4SW_LSE;
            static constexpr uint32_t HSI = RCC_CFGR3_UART4SW_HSI;
        };

        static constexpr uint32_t UART5_SOURCE_SELECTION = RCC_CFGR3_UART5SW;
        static constexpr uint32_t UART5_SOURCE_SELECTION_POSITION = RCC_CFGR3_UART5SW_Pos;
        struct UART5_SOURCES_MASK
        {
            static constexpr uint32_t PCLK = RCC_CFGR3_UART5SW_PCLK;
            static constexpr uint32_t SYSCLK = RCC_CFGR3_UART5SW_SYSCLK;
            static constexpr uint32_t LSE = RCC_CFGR3_UART5SW_LSE;
            static constexpr uint32_t HSI = RCC_CFGR3_UART5SW_HSI;
        };
    };

    struct RCC_AHB_CLOCK_ENABLE
    {
        static constexpr uint32_t DMA1_CLOCK = RCC_AHBENR_DMA1EN;
        static constexpr uint32_t DMA2_CLOCK = RCC_AHBENR_DMA2EN;
        static constexpr uint32_t SRAM_CLOCK = RCC_AHBENR_SRAMEN;
        static constexpr uint32_t FLIFT_CLOCK = RCC_AHBENR_FLITFEN;
        static constexpr uint32_t CRC_CLOCK = RCC_AHBENR_CRCEN;
        static constexpr uint32_t GPIOA_CLOCK = RCC_AHBENR_GPIOAEN;
        static constexpr uint32_t GPIOB_CLOCK = RCC_AHBENR_GPIOBEN;
        static constexpr uint32_t GPIOC_CLOCK = RCC_AHBENR_GPIOCEN;
        static constexpr uint32_t GPIOD_CLOCK = RCC_AHBENR_GPIODEN;
        static constexpr uint32_t GPIOE_CLOCK = RCC_AHBENR_GPIOEEN;
        static constexpr uint32_t GPIOF_CLOCK = RCC_AHBENR_GPIOFEN;
        static constexpr uint32_t TOUCH_SENSING_CONTROLLER_CLOCK = RCC_AHBENR_TSCEN;
        static constexpr uint32_t ADC1_ADC2_CLOCK = RCC_AHBENR_ADC12EN;
        static constexpr uint32_t ADC3_ADC4_CLOCK = RCC_AHBENR_ADC34EN;
    };

    struct RCC_APB2_CLOCK_ENABLE
    {
        static constexpr uint32_t SYSCFG_CLOCK = RCC_APB2ENR_SYSCFGEN;
        static constexpr uint32_t TIMER1_CLOCK = RCC_APB2ENR_TIM1EN;
        static constexpr uint32_t SPI1_CLOCK = RCC_APB2ENR_SPI1EN;
        static constexpr uint32_t TIMER8_CLOCK = RCC_APB2ENR_TIM8EN;
        static constexpr uint32_t USART1_CLOCK = RCC_APB2ENR_USART1EN;
        static constexpr uint32_t TIMER15_CLOCK = RCC_APB2ENR_TIM15EN;
        static constexpr uint32_t TIMER16_CLOCK = RCC_APB2ENR_TIM16EN;
        static constexpr uint32_t TIMER17_CLOCK = RCC_APB2ENR_TIM17EN;
    };

    struct RCC_APB1_CLOCK_ENABLE
    {
        static constexpr uint32_t TIMER2_CLOCK = RCC_APB1ENR_TIM2EN;
        static constexpr uint32_t TIMER3_CLOCK = RCC_APB1ENR_TIM3EN;
        static constexpr uint32_t TIMER4_CLOCK = RCC_APB1ENR_TIM4EN;
        static constexpr uint32_t TIMER6_CLOCK = RCC_APB1ENR_TIM6EN;
        static constexpr uint32_t TIMER7_CLOCK = RCC_APB1ENR_TIM7EN;
        static constexpr uint32_t WINDOW_WATCHDOG_CLOCK = RCC_APB1ENR_WWDGEN;
        static constexpr uint32_t SPI2_CLOCK = RCC_APB1ENR_SPI2EN;
        static constexpr uint32_t SPI3_CLOCK = RCC_APB1ENR_SPI3EN;
        static constexpr uint32_t USART2_CLOCK = RCC_APB1ENR_USART2EN;
        static constexpr uint32_t USART3_CLOCK = RCC_APB1ENR_USART3EN;
        static constexpr uint32_t UART4_CLOCK = RCC_APB1ENR_UART4EN;
        static constexpr uint32_t UART5_CLOCK = RCC_APB1ENR_UART5EN;
        static constexpr uint32_t I2C1_CLOCK = RCC_APB1ENR_I2C1EN;
        static constexpr uint32_t I2C2_CLOCK = RCC_APB1ENR_I2C2EN;
        static constexpr uint32_t USB_CLOCK = RCC_APB1ENR_USBEN;
        static constexpr uint32_t CAN_CLOCK = RCC_APB1ENR_CANEN;
        static constexpr uint32_t POWER_INTERFACE_CLOCK = RCC_APB1ENR_PWREN;
        static constexpr uint32_t DAC1_CLOCK = RCC_APB1ENR_DAC1EN;
    };

};
