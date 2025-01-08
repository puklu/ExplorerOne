#pragma once

#include "defines.hpp"
#include "stm32f303xc.h"


inline const void* aAltFunctionsAdressesPortA[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_A] = {
    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, TIM8, TIM8, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, TIM15, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, TIM15, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, TIM15, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, TIM3, nullptr, TIM8, nullptr, TIM1, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM17, TIM3, nullptr, TIM8, nullptr, TIM1, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, USART1,
    nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, USART1,
    nullptr, TIM15, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM17, nullptr, nullptr, nullptr, nullptr, TIM1, USART1,
    nullptr, nullptr, TIM2, TIM8, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, USART1,
    nullptr, nullptr, TIM4, TIM1, TIM1, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, nullptr, nullptr, nullptr, nullptr, TIM1, USART1,
    nullptr, nullptr, TIM4, TIM1, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, TIM4, TIM1, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, TIM8, TIM1, USART2,
    nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, TIM8, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 
};

inline const void* aAltFunctionsAdressesPortB[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_B] = {
    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, TIM1, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, TIM1, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, TIM4, nullptr, TIM8, nullptr, nullptr, USART2,
    nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, TIM3, nullptr, TIM8, nullptr, nullptr, USART2,
    nullptr, nullptr, TIM17, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, TIM3, TIM8, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, TIM17, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, TIM4, nullptr, nullptr, nullptr, TIM8, USART1,
    nullptr, nullptr, TIM8, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM17, TIM4, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM16, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, TIM8, TIM1, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM17, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, TIM8, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM15, nullptr, nullptr, nullptr, nullptr, TIM1, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, TIM15, TIM15, nullptr, TIM1, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

inline const void* aAltFunctionsAdressesPortC[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_C + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, TIM8, nullptr, TIM8, nullptr},

    {nullptr, nullptr, nullptr, nullptr, TIM8, UART4, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, TIM8, UART4, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, TIM8, UART5, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, TIM1, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

inline const void* aAltFunctionsAdressesPortD[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_D + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, TIM8, nullptr, TIM8, nullptr},

    {nullptr, nullptr, TIM3, nullptr, TIM8, UART5, nullptr, nullptr},

    {nullptr, nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, TIM2, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM4, nullptr, nullptr, nullptr, nullptr, nullptr},
};

inline const void* aAltFunctionsAdressesPortE[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_E + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, TIM4, nullptr, TIM16, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, TIM17, nullptr, nullptr, USART1},

    {nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM3, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, TIM1, nullptr},

    {nullptr, nullptr, TIM1, nullptr, nullptr, nullptr, nullptr, USART3},
};

inline const void* aAltFunctionsAdressesPortF[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_F + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, TIM1, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, TIM15, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, TIM15, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};
