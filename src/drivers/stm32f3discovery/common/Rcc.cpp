#include <unordered_map>
#include "common/assertHandler.hpp"
#include "common/defines.hpp"
#include "Rcc.hpp"


RccImpl* RccImpl::mpInstance = nullptr;
uint32_t RccImpl::SYS_CLK = 0;

RccImpl* RccImpl::GetInstance()
{
    if(mpInstance == nullptr)
    {
        mpInstance = new RccImpl();
    }
    return mpInstance;
}

RccImpl::RccImpl()
    :mpRCC(aRcc::ADDRESS)
{

}

eGeneralStatus RccImpl::SelectSystemClock(eRccClockSource source)
{
    // clear the bits
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH;

    switch (source)
    {
    case eRccClockSource::RCC_CLOCK_SOURCE_HSI:
        mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::HSI;
        break;
    
    case eRccClockSource::RCC_CLOCK_SOURCE_HSE:
        mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::HSE;
        break;

    case eRccClockSource::RCC_CLOCK_SOURCE_PLL:
        mpRCC->CFGR |= aRcc::RCC_CFGR::CLOCK_SELECTION_MASK::PLL;
        break;   
    
    default:
        ASSERT(false);
    }

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
        {eRccClocks::RCC_CLOCK_SOURCE_LSI, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::LSI_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_LSE, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::LSE_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_HSI, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::HSI_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_HSE, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::HSE_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_PLL, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::PLL_ON_MCO},
        {eRccClocks::RCC_CLOCK_SOURCE_SYSCLK, aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::SYSCLK_ON_MCO},
    };
    
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT;

    auto it = clockOnMcoMap.find(clock);

    if(it != clockOnMcoMap.end())
    {
        mpRCC->CFGR |= it->second;
    }

    else
    {
        mpRCC->CFGR |= aRcc::RCC_CFGR::MICRCONTROLLER_CLOCK_OUTPUT_MASK::NO_CLOCK_ON_MCO;

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
    // disable PLL first
    mpRCC->CR &= ~aRcc::RCC_CR::PLL_ON;

    // wait until PLL is ready (PLLRDY is cleared)
    while(mpRCC->CR & aRcc::RCC_CR::PLL_READY);

    // change the desired parameters
    // set multiplication factor

    // a look-up table to find appropriate mask for each case
    const std::unordered_map<ePllMultiplicationFactor, uint32_t> pllMap = {
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_2, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X2},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_3, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X3},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_4, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X4},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_5, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X5},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_6, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X6},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_7, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X7},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_8, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X8},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_9, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X9},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_10, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X10},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_11, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X11},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_12, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X12},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_13, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X13},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_14, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X14},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_15, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X15},
        {ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_16, aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_MASK::INPUT_CLOCK_X16},
    };

    mpRCC->CFGR &= ~aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR;  // clear the bits first

    auto it = pllMap.find(multiplication_factor);

    // found?
    if(it != pllMap.end())
    {
        mpRCC->CFGR |= it->second;
    }

    else
    {
        ASSERT(false);
    }

    // set PLL entry clock source
    mpRCC->CFGR &= ~aRcc::RCC_CFGR::PLL_CLOCK_SOURCE;  // clear the bit first
    mpRCC->CFGR |= aRcc::RCC_CFGR::PLL_CLOCK_SOURCE_MASK::HSI_DIVIDED_BY_2;

    // enable PLL again
    mpRCC->CR |= aRcc::RCC_CR::PLL_ON;

    return eGeneralStatus::SUCCESS;
}

uint32_t RccImpl::GetSysClockFreq()
{
    uint32_t sysClk = 0;

    uint32_t sws =  mpRCC->CFGR & ~aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS;

    if(sws == aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSE)
    {
        // not implemented for HSE
        ASSERT(false);
    }
    else if(sws == aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::HSI)
    {
        sysClk = HSI_FREQ;
    }
    else if(sws == aRcc::RCC_CFGR::SYSTEM_CLOCK_SWITCH_STATUS_MASK::PLL)
    {
        uint32_t pllMul = ((mpRCC->CFGR & aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR) >> aRcc::RCC_CFGR::PLL_MULTIPLICATION_FACTOR_POSITION) + 2;
        uint32_t pllSrc = 0;
        if((mpRCC->CFGR & aRcc::RCC_CFGR::PLL_CLOCK_SOURCE) == 0)
        {
            pllSrc = HSI_FREQ/2;
        }
        else
        {
            // not implemented for HSE
            ASSERT(false);
        }
        sysClk = pllSrc*pllMul;

        ASSERT(sysClk >= MIN_PLL_FREQ && sysClk <= MAX_PLL_FREQ_WITH_HSI);
    }

    return sysClk;
}

eGeneralStatus RccImpl::SetAhbPrescaler(eAhbPrescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eAhbPrescaler, uint32_t> prescalerMap = {
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_1, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_1},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_2, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_2},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_4, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_4},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_8, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_8},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_16, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_16},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_64, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_64},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_128, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_128},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_256, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_256},
        {eAhbPrescaler::SYSCLK_DIVIDED_BY_512, aRcc::RCC_CFGR::AHB_PRESCALER_MASK::DIVIDE_BY_512},
        
    };

    // clear all the bits first
    mpRCC->CFGR &= ~ aRcc::RCC_CFGR::AHB_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if(it != prescalerMap.end())
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
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::AHB_PRESCALER) >> aRcc::RCC_CFGR::AHB_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if(prescalerValue >= 0b1000)
    {
        switch (prescalerValue)
        {
        case 0b1000: divisor = 2; break;
        case 0b1001: divisor = 4; break;
        case 0b1010: divisor = 8; break;
        case 0b1011: divisor = 16; break;
        case 0b1100: divisor = 64; break;
        case 0b1101: divisor = 128; break;
        case 0b1110: divisor = 256; break;
        case 0b1111: divisor = 512; break;
        default: divisor = 1; break;
        }
    }

    uint32_t sysClk = GetSysClockFreq();
    uint32_t hclk = sysClk/divisor;

    return hclk;
}

eGeneralStatus RccImpl::SetApb1Prescaler(eApb1Apb2Prescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eApb1Apb2Prescaler, uint32_t> prescalerMap = {
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1, aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_1},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_2, aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_2},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_4, aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_4},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_8, aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_8},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_16, aRcc::RCC_CFGR::APB1_PRESCALER_MASK::DIVIDE_BY_16},
    };

    // clear all the bits first
    mpRCC->CFGR &= ~ aRcc::RCC_CFGR::APB1_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if(it != prescalerMap.end())
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
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::APB1_PRESCALER) >> aRcc::RCC_CFGR::APB1_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if(prescalerValue >= 0b100)
    {
        switch (prescalerValue)
        {
        case 0b100: divisor = 2; break;
        case 0b101: divisor = 4; break;
        case 0b110: divisor = 8; break;
        case 0b111: divisor = 16; break;
        default: divisor = 1; break;
        }
    }

    uint32_t hclk = GetAhbFrequency();
    uint32_t pclk1 = hclk/divisor;

    return pclk1;
}

eGeneralStatus RccImpl::SetApb2Prescaler(eApb1Apb2Prescaler psc)
{
    // a look-up table to find appropriate mask for each case
    const std::unordered_map<eApb1Apb2Prescaler, uint32_t> prescalerMap = {
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1, aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_1},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_2, aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_2},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_4, aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_4},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_8, aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_8},
        {eApb1Apb2Prescaler::HCLK_DIVIDED_BY_16, aRcc::RCC_CFGR::APB2_PRESCALER_MASK::DIVIDE_BY_16},
    };

    // clear all the bits first
    mpRCC->CFGR &= ~ aRcc::RCC_CFGR::APB2_PRESCALER;

    auto it = prescalerMap.find(psc);

    // found?
    if(it != prescalerMap.end())
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
    uint32_t prescalerValue = (mpRCC->CFGR & aRcc::RCC_CFGR::APB2_PRESCALER) >> aRcc::RCC_CFGR::APB2_PRESCALER_POSITION;
    uint32_t divisor = 1;

    if(prescalerValue >= 0b100)
    {
        switch (prescalerValue)
        {
        case 0b100: divisor = 2; break;
        case 0b101: divisor = 4; break;
        case 0b110: divisor = 8; break;
        case 0b111: divisor = 16; break;
        default: divisor = 1; break;
        }
    }

    uint32_t hclk = GetAhbFrequency();
    uint32_t pclk1 = hclk/divisor;

    return pclk1;   
}
