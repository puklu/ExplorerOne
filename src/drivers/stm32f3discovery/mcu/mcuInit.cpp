#include "mcuInit.hpp"

#include "common/Delay.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/ISystick.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/common/DefaultPinConfigs.hpp"
#include "drivers/stm32f3discovery/common/IrqHandlers.cpp"
#include "drivers/stm32f3discovery/common/Rcc.hpp"
#include "drivers/stm32f3discovery/common/SysTickImpl.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"
#include "stm32f303xc.h"

static bool isSystemInitialized = false;

void SystemInit()
{
// TODO: Handle watchdog here
// WWDG->CR &= ~ (1<<7);  //Disable watchdog

// DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;  // Freeze IWDG in debug mode
// DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;  // Freeze WWDG in debug mode

/* Enable FPU ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |=
        ((3UL << 20U) | (3UL << 22U)); /* set CP10 and CP11 Full Access */
#endif
}

bool IsSystemInitialized()
{
    return isSystemInitialized;
}

void SetupRcc()
{
    RccImpl* gpRcc = RccImpl::GetInstance();
    gpRcc->SetUpPll(ePllMultiplicationFactor::MULTIPLY_INPUT_CLK_BY_16);
    gpRcc->SelectSystemClock(
        eRccClockSource::RCC_CLOCK_SOURCE_HSI);  // TODO: Change to PLL
    gpRcc->SelectMcoClock(eRccClocks::RCC_CLOCK_SOURCE_SYSCLK);
    gpRcc->SetAhbPrescaler(eAhbPrescaler::SYSCLK_DIVIDED_BY_1);
    gpRcc->SetApb1Prescaler(eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1);
    gpRcc->SetApb2Prescaler(eApb1Apb2Prescaler::HCLK_DIVIDED_BY_1);
    gpRcc->SelectTim1Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK2);
    gpRcc->SelectTim8Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK2);
    gpRcc->SelectUsart1Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK2);
    gpRcc->SelectUsart2Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK1);
    gpRcc->SelectUsart3Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK1);
    gpRcc->SelectUart4Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK1);
    gpRcc->SelectUart5Clock(eRccClocks::RCC_CLOCK_SOURCE_PCLK1);
    gpRcc->SelectI2c1Clock(eRccClocks::RCC_CLOCK_SOURCE_SYSCLK);
    gpRcc->SelectI2c2Clock(eRccClocks::RCC_CLOCK_SOURCE_SYSCLK);
}

void PostSystemInit()
{
    SetupRcc();

    InitializeConsolePrinting();

    InitializeDelaySystem();

    InitializeSystick();

    isSystemInitialized = true;

    TRACE_LOG("Post System Init done");
}

void InitializeConsolePrinting()
{
    if (!activePrintUsartPin)
    {
        UsartPinInitStruct pinInit = DefaultPinConfigs::Uart4_Tx_Af5_115200;

        [[maybe_unused]] std::shared_ptr<PinBase> const usart_print_pin =
            PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_PRINTING_USART,
                                  pinInit);
        std::dynamic_pointer_cast<UsartPin>(usart_print_pin)->Init();
    }
}

void InitializeDelaySystem()
{
    // timer to use for delay functionality
    static BasicTimerConfig            delayTimerConfig;
    static std::unique_ptr<BasicTimer> gpDelayTimer =
        std::make_unique<BasicTimer>(delayTimerConfig);

    gpDelayTimer->Init();

    // // Initialize the Delay singleton
    Delay::Init(*gpDelayTimer);
}

void InitializeSystick()
{
    ISysTick* gpSystick = SysTickImpl::GetInstance();
    gpSystick->SystickSetup(1000, RccImpl::GetInstance()->GetAhbFrequency());
}
