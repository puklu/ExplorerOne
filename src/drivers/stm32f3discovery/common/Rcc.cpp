#include "Rcc.hpp"

#include <unordered_map>

#include "common/assertHandler.hpp"
#include "common/defines.hpp"

RccImpl* RccImpl::GetInstance()
{
    static RccImpl instance;
    return &instance;
}

RccImpl::RccImpl() : mpRCC(aRcc::ADDRESS)
{
}

eGeneralStatus RccImpl::SelectSystemClock(eRccClockSource source)
{
    // clear the bits
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH;

    switch (source)
    {
        case eRccClockSource::RCC_CLOCK_SOURCE_HSI:
            while (!(mpRCC->CR & aRcc::RCC_CR::HSE_READY))
                ;
            mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::HSI;
            break;

        case eRccClockSource::RCC_CLOCK_SOURCE_HSE:
            while (!(mpRCC->CR & aRcc::RCC_CR::HSE_READY))
                ;
            mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::HSE;
            break;

        case eRccClockSource::RCC_CLOCK_SOURCE_PLL:
            while (!(mpRCC->CR & aRcc::RCC_CR::PLL_READY))
                ;
            mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::PLL;
            break;

        default:
            ASSERT(false);
    }

    // wait until system clock is complete
    uint32_t sws = (mpRCC->CFGR & aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS) >>
                   aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION;

    const uint32_t expected_sws_bits =
        (source == eRccClockSource::RCC_CLOCK_SOURCE_HSE)
            ? (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSE >>
               aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION)
        : (source == eRccClockSource::RCC_CLOCK_SOURCE_HSI)
            ? (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSI >>
               aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION)

            : (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::PLL >>
               aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION);

    while ((sws != expected_sws_bits))
        ;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SwitchOnClocks([[maybe_unused]] eRccClocks clocks)
{
    return eGeneralStatus::FAILURE;
}

eGeneralStatus RccImpl::SwitchOffClocks([[maybe_unused]] eRccClocks clocks)
{
    return eGeneralStatus::FAILURE;
}

eGeneralStatus RccImpl::SelectMcoClock(eRccClocks clock)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eRccClocks, uint32_t> clockOnMcoMap = {
        {eRccClocks::RCC_CLOCK_SOURCE_LSI,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::LSI_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_LSE,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::LSE_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_HSI,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::HSI_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_HSE,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::HSE_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_PLL,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::PLL_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_SYSCLK,
         aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::SYSCLK_ON_MCO},
    };

    mpRCC->CFGR &= ~aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT;

    auto it = clockOnMcoMap.find(clock);

    if (it != clockOnMcoMap.end())
    {
        mpRCC->CFGR |= it->second;
    }

    else
    {
        mpRCC->CFGR |=
            aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::NO_CLOCK_ON_MCO;
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::TurnRtcOnOff()
{
    return eGeneralStatus::FAILURE;
}

eGeneralStatus RccImpl::SelectRtcClock([[maybe_unused]] eRccClocks clock)
{
    return eGeneralStatus::FAILURE;
}

eGeneralStatus RccImpl::SetUpPll(ePllMultiplicationFactor multiplication_factor)
{
    ASSERT(mpRCC != nullptr);

    // 0. enable HSI and wait for it to be ready
    mpRCC->CR |= aRcc::RCC_CR::HSI_ON;
    while (!(mpRCC->CR & aRcc::RCC_CR::HSI_READY))
        ;

    // 1. disable PLL first
    mpRCC->CR &= ~aRcc::RCC_CR::PLL_ON;

    // wait until PLL is ready (PLLRDY is cleared)
    while (mpRCC->CR & aRcc::RCC_CR::PLL_READY)
        ;

    // 2. set PLL entry clock source
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::PLL_CLOCK_SOURCE;  // clear the bit first
    mpRCC->CFGR |= aRcc::RCC_CFGR::PLL_CLOCK_SOURCE_MASK::HSI_DIVIDED_BY_2;

    // 3. set multiplication factor

    // a look-up table to find appropriate mask for each case
    const std::unordered_map<ePllMultiplicationFactor, uint32_t> pllMap = {
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_2,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X2},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_3,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X3},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_4,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X4},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_5,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X5},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_6,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X6},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_7,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X7},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_8,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X8},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_9,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X9},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_10,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X10},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_11,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X11},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_12,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X12},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_13,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X13},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_14,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X14},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_15,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X15},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_16,
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X16},
    };

    mpRCC->CFGR &=
        ~aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR;  // clear the bits first

    auto it = pllMap.find(multiplication_factor);

    // found?
    if (it != pllMap.end())
    {
        mpRCC->CFGR |= it->second;
    }

    else
    {
        ASSERT(false);
    }

    // 4. enable PLL again
    mpRCC->CR |= aRcc::RCC_CR::PLL_ON;

    // 5. wait untill PLLRDY is set
    while (!(mpRCC->CR & aRcc::RCC_CR::PLL_READY))
        ;

    // 6. configure latency states because of higher clock speed
    ConfigureFlashLatency();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::ConfigureFlashLatency()
{
    // enable flash prefetch buffer
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    // clear existing latency bits
    FLASH->ACR &= ~FLASH_ACR_LATENCY;

    // TODO: Numbe rof states can be changed based on clock freq??
    // set latency to 2 states
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetPllFreq()
{
    uint32_t pllMul =
        ((mpRCC->CFGR & aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR) >>
         aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_POSITION) +
        2;
    uint32_t pllSrc = 0;
    if ((mpRCC->CFGR & aRcc::RCC_CFGR::PLL_CLOCK_SOURCE) == 0)
    {
        pllSrc = HSI_FREQ / 2;
    }
    else
    {
        // not implemented for HSE
        ASSERT(false);
    }
    uint32_t pllFreq = pllSrc * pllMul;

    ASSERT(pllFreq >= MIN_PLL_FREQ && pllFreq <= MAX_PLL_FREQ_WITH_HSI);

    return pllFreq;
}

uint32_t RccImpl::GetSysClockFreq()
{
    uint32_t sysClk = 0;

    uint32_t sws = (mpRCC->CFGR & aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS) >>
                   aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION;

    if (sws == (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSE >>
                aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION))
    {
        // not implemented for HSE
        ASSERT(false);
    }
    else if (sws == (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSI >>
                     aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION))
    {
        sysClk = HSI_FREQ;
    }
    else if (sws == (aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::PLL >>
                     aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_POSITION))
    {
        sysClk = GetPllFreq();
    }

    return sysClk;
}

eGeneralStatus RccImpl::SetAhbPrescaler(eAhbPrescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eAhbPrescaler, uint32_t> prescalerMap = {
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_1,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_1},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_2,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_2},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_4,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_4},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_8,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_8},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_16,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_16},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_64,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_64},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_128,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_128},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_256,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_256},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_512,
         aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_512},

    };

    // clear all the bits first
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::AHB_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if (it != prescalerMap.end())
    {
        mpRCC->CFGR |= it->second;
    }
    else
    {
        ASSERT(false);
    }

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetAhbFrequency()
{
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::AHB_PRESCALER) >>
                              aRcc::RCC_CFGR::AHB_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if (prescalerValue >= 0b1000)
    {
        switch (prescalerValue)
        {
            case 0b1000:
                divisor = 2;
                break;
            case 0b1001:
                divisor = 4;
                break;
            case 0b1010:
                divisor = 8;
                break;
            case 0b1011:
                divisor = 16;
                break;
            case 0b1100:
                divisor = 64;
                break;
            case 0b1101:
                divisor = 128;
                break;
            case 0b1110:
                divisor = 256;
                break;
            case 0b1111:
                divisor = 512;
                break;
            default:
                divisor = 1;
                break;
        }
    }

    uint32_t sysClk = GetSysClockFreq();
    uint32_t hclk   = sysClk / divisor;

    return hclk;
}

eGeneralStatus RccImpl::SetApb1Prescaler(eApb1Apb2Prescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eApb1Apb2Prescaler, uint32_t> prescalerMap = {
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1,
         aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_1},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_2,
         aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_2},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_4,
         aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_4},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_8,
         aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_8},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_16,
         aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_16},
    };

    // clear all the bits first
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::APB1_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if (it != prescalerMap.end())
    {
        mpRCC->CFGR |= it->second;
    }
    else
    {
        ASSERT(false);
    }

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetApb1Frequency()
{
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::APB1_PRESCALER) >>
                              aRcc::RCC_CFGR::APB1_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if (prescalerValue >= 0b100)
    {
        switch (prescalerValue)
        {
            case 0b100:
                divisor = 2;
                break;
            case 0b101:
                divisor = 4;
                break;
            case 0b110:
                divisor = 8;
                break;
            case 0b111:
                divisor = 16;
                break;
            default:
                divisor = 1;
                break;
        }
    }

    uint32_t hclk  = GetAhbFrequency();
    uint32_t pclk1 = hclk / divisor;

    return pclk1;
}

eGeneralStatus RccImpl::SetApb2Prescaler(eApb1Apb2Prescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eApb1Apb2Prescaler, uint32_t> prescalerMap = {
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1,
         aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_1},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_2,
         aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_2},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_4,
         aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_4},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_8,
         aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_8},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_16,
         aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_16},
    };

    // clear all the bits first
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::APB2_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if (it != prescalerMap.end())
    {
        mpRCC->CFGR |= it->second;
    }
    else
    {
        ASSERT(false);
    }

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetApb2Frequency()
{
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::APB2_PRESCALER) >>
                              aRcc::RCC_CFGR::APB2_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if (prescalerValue >= 0b100)
    {
        switch (prescalerValue)
        {
            case 0b100:
                divisor = 2;
                break;
            case 0b101:
                divisor = 4;
                break;
            case 0b110:
                divisor = 8;
                break;
            case 0b111:
                divisor = 16;
                break;
            default:
                divisor = 1;
                break;
        }
    }

    uint32_t hclk  = GetAhbFrequency();
    uint32_t pclk1 = hclk / divisor;

    return pclk1;
}

uint32_t RccImpl::FindUsartClockSourceMask(eRccClocks clock,
                                           uint8_t    usart_number)
{
    switch (usart_number)
    {
        case 1:
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK2:
                    return aRcc::RCC_CFGR3::USART1_SOURCES_MASK::PCLK2;
                case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
                    return aRcc::RCC_CFGR3::USART1_SOURCES_MASK::SYSCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_HSI:
                    return aRcc::RCC_CFGR3::USART1_SOURCES_MASK::HSI;
                case eRccClocks::RCC_CLOCK_SOURCE_LSE:
                    return aRcc::RCC_CFGR3::USART1_SOURCES_MASK::LSE;
                default:
                    ASSERT(false);
                    return 0;
            }

        case 2:
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK1:
                    return aRcc::RCC_CFGR3::USART2_SOURCES_MASK::PCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
                    return aRcc::RCC_CFGR3::USART2_SOURCES_MASK::SYSCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_HSI:
                    return aRcc::RCC_CFGR3::USART2_SOURCES_MASK::HSI;
                case eRccClocks::RCC_CLOCK_SOURCE_LSE:
                    return aRcc::RCC_CFGR3::USART2_SOURCES_MASK::LSE;
                default:
                    ASSERT(false);
                    return 0;
            }

        case 3:
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK1:
                    return aRcc::RCC_CFGR3::USART3_SOURCES_MASK::PCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
                    return aRcc::RCC_CFGR3::USART3_SOURCES_MASK::SYSCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_HSI:
                    return aRcc::RCC_CFGR3::USART3_SOURCES_MASK::HSI;
                case eRccClocks::RCC_CLOCK_SOURCE_LSE:
                    return aRcc::RCC_CFGR3::USART3_SOURCES_MASK::LSE;
                default:
                    ASSERT(false);
                    return 0;
            }

        case 4:
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK1:
                    return aRcc::RCC_CFGR3::UART4_SOURCES_MASK::PCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
                    return aRcc::RCC_CFGR3::UART4_SOURCES_MASK::SYSCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_HSI:
                    return aRcc::RCC_CFGR3::UART4_SOURCES_MASK::HSI;
                case eRccClocks::RCC_CLOCK_SOURCE_LSE:
                    return aRcc::RCC_CFGR3::UART4_SOURCES_MASK::LSE;
                default:
                    ASSERT(false);
                    return 0;
            }

        case 5:
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK1:
                    return aRcc::RCC_CFGR3::UART5_SOURCES_MASK::PCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
                    return aRcc::RCC_CFGR3::UART5_SOURCES_MASK::SYSCLK;
                case eRccClocks::RCC_CLOCK_SOURCE_HSI:
                    return aRcc::RCC_CFGR3::UART5_SOURCES_MASK::HSI;
                case eRccClocks::RCC_CLOCK_SOURCE_LSE:
                    return aRcc::RCC_CFGR3::UART5_SOURCES_MASK::LSE;
                default:
                    ASSERT(false);
                    return 0;
            }

        default:
            ASSERT(false);
            return 0;
    }
}

eGeneralStatus RccImpl::SelectUsart1Clock(eRccClocks clock)
{
    uint32_t mask = FindUsartClockSourceMask(clock, 1);
    mpRCC->CFGR3 &=
        ~aRcc::RCC_CFGR3::USART1_SOURCE_SELECTION;  // clear all bits first
    mpRCC->CFGR3 |= mask;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectUsart2Clock(eRccClocks clock)
{
    uint32_t mask = FindUsartClockSourceMask(clock, 2);
    mpRCC->CFGR3 &=
        ~aRcc::RCC_CFGR3::USART2_SOURCE_SELECTION;  // clear all bits first
    mpRCC->CFGR3 |= mask;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectUsart3Clock(eRccClocks clock)
{
    uint32_t mask = FindUsartClockSourceMask(clock, 3);
    mpRCC->CFGR3 &=
        ~aRcc::RCC_CFGR3::USART3_SOURCE_SELECTION;  // clear all bits first
    mpRCC->CFGR3 |= mask;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectUart4Clock(eRccClocks clock)
{
    uint32_t mask = FindUsartClockSourceMask(clock, 4);
    mpRCC->CFGR3 &=
        ~aRcc::RCC_CFGR3::UART4_SOURCE_SELECTION;  // clear all bits first
    mpRCC->CFGR3 |= mask;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectUart5Clock(eRccClocks clock)
{
    uint32_t mask = FindUsartClockSourceMask(clock, 5);
    mpRCC->CFGR3 &=
        ~aRcc::RCC_CFGR3::UART5_SOURCE_SELECTION;  // clear all bits first
    mpRCC->CFGR3 |= mask;

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetUsartClockFreq(uint8_t usart_number)
{
    uint32_t regValue  = 0;
    uint32_t PCLKxFreq = 0;

    switch (usart_number)
    {
        case 1:
            regValue =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::USART1_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::USART1_SOURCE_SELECTION_POSITION;
            PCLKxFreq = GetApb2Frequency();
            break;

        case 2:
            regValue =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::USART2_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::USART2_SOURCE_SELECTION_POSITION;
            PCLKxFreq = GetApb1Frequency();
            break;

        case 3:
            regValue =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::USART3_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::USART3_SOURCE_SELECTION_POSITION;
            PCLKxFreq = GetApb1Frequency();
            break;

        case 4:
            regValue =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::UART4_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::UART4_SOURCE_SELECTION_POSITION;
            PCLKxFreq = GetApb1Frequency();
            break;

        case 5:
            regValue =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::UART5_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::UART5_SOURCE_SELECTION_POSITION;
            PCLKxFreq = GetApb1Frequency();
            break;

        default:
            ASSERT(false);
    }

    switch (regValue)
    {
        case 0b00:
            return PCLKxFreq;

        case 0b01:
            return GetSysClockFreq();

        case 0b10:
            return HSI_FREQ;

        case 0b11:
            ASSERT(false);  // LSE not implemented
            return 0;

        default:
            ASSERT(false);
            return 0;
    }
}

uint32_t RccImpl::GetUsart1ClockFreq()
{
    return GetUsartClockFreq(1);
}

uint32_t RccImpl::GetUsart2ClockFreq()
{
    return GetUsartClockFreq(2);
}

uint32_t RccImpl::GetUsart3ClockFreq()
{
    return GetUsartClockFreq(3);
}

uint32_t RccImpl::GetUart4ClockFreq()
{
    return GetUsartClockFreq(4);
}

uint32_t RccImpl::GetUart5ClockFreq()
{
    return GetUsartClockFreq(5);
}

eGeneralStatus RccImpl::SetAdcPrescaler()
{
    return eGeneralStatus::FAILURE;
}

eGeneralStatus RccImpl::SelectAdcClock()
{
    return eGeneralStatus::FAILURE;
}

uint32_t RccImpl::GetAdcClockFreq()
{
    return 0;
}

eGeneralStatus RccImpl::SelectRtcClock()
{
    return eGeneralStatus::FAILURE;
}

uint32_t RccImpl::GetRtcClockFreq()
{
    return 0;
}

eGeneralStatus RccImpl::SelectTim_1_8_Clock(eRccClocks clock, uint8_t timer_num)
{
    ASSERT(timer_num == 1 or timer_num == 8);

    if ((GetSysClockFreq() != GetPllFreq()) ||
        (GetSysClockFreq() != GetAhbFrequency()) ||
        (GetSysClockFreq() != GetApb2Frequency()))
    {
        return eGeneralStatus::FAILURE;  // cannot change the bit
    }

    switch (timer_num)
    {
        case 1:
            mpRCC->CFGR3 &= ~aRcc::RCC_CFGR3::TIM1_SOURCE_SELECTION;
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK2:
                    mpRCC->CFGR3 |= aRcc::RCC_CFGR3::TIM1_SOURCES_MASK::PCLK2;
                    break;
                case eRccClocks::RCC_CLOCK_SOURCE_PLL:
                    mpRCC->CFGR3 |= aRcc::RCC_CFGR3::TIM1_SOURCES_MASK::PLL;
                    break;
                default:
                    ASSERT(false);
                    break;
            }
            break;

        case 8:
            mpRCC->CFGR3 &= ~aRcc::RCC_CFGR3::TIM8_SOURCE_SELECTION;
            switch (clock)
            {
                case eRccClocks::RCC_CLOCK_SOURCE_PCLK2:
                    mpRCC->CFGR3 |= aRcc::RCC_CFGR3::TIM8_SOURCES_MASK::PCLK2;
                    break;
                case eRccClocks::RCC_CLOCK_SOURCE_PLL:
                    mpRCC->CFGR3 |= aRcc::RCC_CFGR3::TIM8_SOURCES_MASK::PLL;
                    break;
                default:
                    ASSERT(false);
                    break;
            }
            break;

        default:
            ASSERT(false);
            break;
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectTim1Clock(eRccClocks clock)
{
    SelectTim_1_8_Clock(clock, 1);
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectTim8Clock(eRccClocks clock)
{
    SelectTim_1_8_Clock(clock, 8);
    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetTim_1_8_15_16_17_ClockFreq(uint8_t timer_num)
{
    ASSERT(timer_num == 1 or timer_num == 8 or timer_num == 15 or
           timer_num == 16 or timer_num == 17);

    uint32_t freq = GetApb2Frequency();

    // if APB2 prescaler is 1 then PCLK2, else 2 x PCLK2
    uint32_t apb2Prescaler = (mpRCC->CFGR & aRcc::RCC_CFGR::APB2_PRESCALER) >>
                             aRcc::RCC_CFGR::APB2_PRESCALER_POSITION;
    if (apb2Prescaler != 0b00)
    {
        freq *= 2;
    }

    if ((GetSysClockFreq() != GetPllFreq()) ||
        (GetSysClockFreq() != GetAhbFrequency()) ||
        (GetSysClockFreq() != GetApb2Frequency()))
    {
        return freq;
    }

    uint32_t selectedClockBit = 0;
    switch (timer_num)
    {
        case 1:
            selectedClockBit =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::TIM1_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::TIM1_SOURCE_SELECTION_POSITION;
            break;

        case 8:
            selectedClockBit =
                (mpRCC->CFGR3 & aRcc::RCC_CFGR3::TIM8_SOURCE_SELECTION) >>
                aRcc::RCC_CFGR3::TIM8_SOURCE_SELECTION_POSITION;
            break;

        case 15:
        case 16:
        case 17:
            return freq;

        default:
            ASSERT(false);
            break;
    }

    switch (selectedClockBit)
    {
        case 0:
            return freq;

        case 1:
            return GetPllFreq();

        default:
            ASSERT(false);
            return 0;
    }
}

uint32_t RccImpl::GetTim_2_3_4_6_7_ClockFreq(uint8_t timer_num)
{
    ASSERT(timer_num == 2 or timer_num == 3 or timer_num == 4 or
           timer_num == 6 or timer_num == 7);

    uint32_t freq = GetApb1Frequency();

    // if APB1 prescaler is 1 then PCLK1, else 2 x PCLK1
    uint32_t apb1Prescaler = (mpRCC->CFGR & aRcc::RCC_CFGR::APB1_PRESCALER) >>
                             aRcc::RCC_CFGR::APB1_PRESCALER_POSITION;
    if (apb1Prescaler != 0b00)
    {
        freq *= 2;
    }

    if ((GetSysClockFreq() != GetPllFreq()) ||
        (GetSysClockFreq() != GetAhbFrequency()) ||
        (GetSysClockFreq() != GetApb1Frequency()))
    {
        return freq;
    }

    return freq;
}

uint32_t RccImpl::GetTim1ClockFreq()
{
    return GetTim_1_8_15_16_17_ClockFreq(1);
}

uint32_t RccImpl::GetTim8ClockFreq()
{
    return GetTim_1_8_15_16_17_ClockFreq(8);
}

uint32_t RccImpl::GetTim15ClockFreq()
{
    return GetTim_1_8_15_16_17_ClockFreq(15);
}

uint32_t RccImpl::GetTim16ClockFreq()
{
    return GetTim_1_8_15_16_17_ClockFreq(16);
}

uint32_t RccImpl::GetTim17ClockFreq()
{
    return GetTim_1_8_15_16_17_ClockFreq(17);
}

uint32_t RccImpl::GetTim2ClockFreq()
{
    return GetTim_2_3_4_6_7_ClockFreq(2);
}

uint32_t RccImpl::GetTim3ClockFreq()
{
    return GetTim_2_3_4_6_7_ClockFreq(3);
}

uint32_t RccImpl::GetTim4ClockFreq()
{
    return GetTim_2_3_4_6_7_ClockFreq(4);
}

uint32_t RccImpl::GetTim6ClockFreq()
{
    return GetTim_2_3_4_6_7_ClockFreq(6);
}

uint32_t RccImpl::GetTim7ClockFreq()
{
    return GetTim_2_3_4_6_7_ClockFreq(7);
}

eGeneralStatus RccImpl::SelectI2cClock(eRccClocks clock, uint8_t i2c_number)
{
    ASSERT(i2c_number == 1 || i2c_number == 2);

    struct I2cClockConfig
    {
        uint32_t selectionMask;
        uint32_t hsiMask;
        uint32_t sysclkMask;
    };

    const std::unordered_map<uint8_t, I2cClockConfig> configMap = {
        {1,
         {aRcc::RCC_CFGR3::I2C1_SOURCE_SELECTION,
          aRcc::RCC_CFGR3::I2C1_SOURCES_MASK::HSI,
          aRcc::RCC_CFGR3::I2C1_SOURCES_MASK::SYSCLK}},
        {2,
         {aRcc::RCC_CFGR3::I2C2_SOURCE_SELECTION,
          aRcc::RCC_CFGR3::I2C2_SOURCES_MASK::HSI,
          aRcc::RCC_CFGR3::I2C2_SOURCES_MASK::SYSCLK}}};

    auto it = configMap.find(i2c_number);

    // not found??
    if (it == configMap.end())
    {
        ASSERT(false);
    }

    // clear the bit first
    mpRCC->CFGR3 &= ~it->second.selectionMask;

    // select the source
    switch (clock)
    {
        case eRccClocks::RCC_CLOCK_SOURCE_HSI:
            mpRCC->CFGR3 |= it->second.hsiMask;
            break;
        case eRccClocks::RCC_CLOCK_SOURCE_SYSCLK:
            mpRCC->CFGR3 |= it->second.sysclkMask;
            break;
        default:
            ASSERT(false);
            break;
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectI2c1Clock(eRccClocks clock)
{
    SelectI2cClock(clock, 1);
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::SelectI2c2Clock(eRccClocks clock)
{
    SelectI2cClock(clock, 2);
    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetI2cClockFreq(uint8_t i2c_number)
{
    ASSERT(i2c_number == 1 || i2c_number == 2);

    struct I2cClockConfig
    {
        uint32_t selectionMask;
        uint32_t selectionPosition;
    };

    const std::unordered_map<uint8_t, I2cClockConfig> configMap = {
        {1,
         {aRcc::RCC_CFGR3::I2C1_SOURCE_SELECTION,
          aRcc::RCC_CFGR3::I2C1_SOURCE_SELECTION_POSITION}},
        {2,
         {aRcc::RCC_CFGR3::I2C2_SOURCE_SELECTION,
          aRcc::RCC_CFGR3::I2C2_SOURCE_SELECTION_POSITION}}};

    auto it = configMap.find(i2c_number);

    // not found??
    if (it == configMap.end())
    {
        ASSERT(false);
    }

    uint32_t selectedClock = (mpRCC->CFGR3 & it->second.selectionMask) >>
                             it->second.selectionPosition;

    switch (selectedClock)
    {
        case 0:
            return HSI_FREQ;

        case 1:
            return GetSysClockFreq();

        default:
            return 0;
    }
}

uint32_t RccImpl::GetI2c1ClockFreq()
{
    return GetI2cClockFreq(1);
}

uint32_t RccImpl::GetI2c2ClockFreq()
{
    return GetI2cClockFreq(2);
}

eGeneralStatus RccImpl::EnableApb1Tim2()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::TIMER2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Tim3()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::TIMER3_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Tim4()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::TIMER4_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Tim6()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::TIMER6_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Tim7()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::TIMER7_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Wwdg()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::WINDOW_WATCHDOG_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Spi2()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::SPI2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Spi3()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::SPI3_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Usart2()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::USART2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Usart3()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::USART3_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Uart4()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::UART4_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Uart5()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::UART5_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1I2c1()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::I2C1_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1I2c2()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::I2C2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Usb()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::USB_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb1Can()
{
    mpRCC->APB1ENR |= aRcc::RCC_APB1_CLOCK_ENABLE::CAN_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Tim1()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::TIMER1_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Spi1()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::SPI1_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Tim8()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::TIMER8_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Usart1()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::USART1_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Tim15()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::TIMER15_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Tim16()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::TIMER16_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableApb2Tim17()
{
    mpRCC->APB2ENR |= aRcc::RCC_APB2_CLOCK_ENABLE::TIMER17_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbDma1()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::DMA1_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbDma2()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::DMA2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioA()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOA_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioB()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOB_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioC()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOC_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioD()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOD_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioE()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOE_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbGpioF()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::GPIOF_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbAdc12()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::ADC1_ADC2_CLOCK;
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus RccImpl::EnableAhbAdc34()
{
    mpRCC->AHBENR |= aRcc::RCC_AHB_CLOCK_ENABLE::ADC3_ADC4_CLOCK;
    return eGeneralStatus::SUCCESS;
}
