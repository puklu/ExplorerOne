#pragma once

#include "defines.hpp"
#include "stm32f303xc.h"


const void* aAltFunctionsAdressesPortA[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_A] = {
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 
};

const void* aAltFunctionsAdressesPortB[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_B] = {
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

const void* aAltFunctionsAdressesPortC[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_C + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, UART4, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, UART4, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, UART5, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

const void* aAltFunctionsAdressesPortD[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_D + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, UART5, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART2},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

const void* aAltFunctionsAdressesPortE[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_E + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART1},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},
};

const void* aAltFunctionsAdressesPortF[IO_PIN_COUNT_PER_PORT][IO_ALT_FUNC_COUNT_FOR_PORT_F + 1] = {
    // The alternate functions in datasheet start from AF1 instead of
    // AF0. But an extra nullptr is added at index 0 of each row here
    // so that the code using this doesnt need to have a -1 
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, USART3},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};
