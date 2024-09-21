/**
 * @file File for all arrays used to hold address and masks 
 * defined in header file provided by stm so that the arrays
 * can be conveniently used by the interface without having to
 * deal with crude registers. 
*/

#pragma once

#include "stm32f303xc.h"
#include "defines.hpp"


namespace IO{

static volatile GPIO_TypeDef *const aPorts[IO_PORT_COUNT] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
};

static volatile int const aPortEnableRegisters[IO_PORT_COUNT] = {
    RCC_AHBENR_GPIOAEN, RCC_AHBENR_GPIOBEN, RCC_AHBENR_GPIOCEN,
    RCC_AHBENR_GPIODEN, RCC_AHBENR_GPIOEEN, RCC_AHBENR_GPIOFEN};

static volatile long unsigned int const
    aModeRegisterBits[IO_BITS_PER_REGISTER] = {
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

static volatile long unsigned int const
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

static volatile long unsigned int const
    aInputDataRegisterBits[IO_PIN_COUNT_PER_PORT] = {
        GPIO_IDR_0,  GPIO_IDR_1,  GPIO_IDR_2,  GPIO_IDR_3,
        GPIO_IDR_4,  GPIO_IDR_5,  GPIO_IDR_6,  GPIO_IDR_7,
        GPIO_IDR_8,  GPIO_IDR_9,  GPIO_IDR_10, GPIO_IDR_11,
        GPIO_IDR_12, GPIO_IDR_13, GPIO_IDR_14, GPIO_IDR_15,
};

static volatile long unsigned int const
    aOutputDataRegisterBits[IO_PIN_COUNT_PER_PORT] = {
        GPIO_ODR_0,  GPIO_ODR_1,  GPIO_ODR_2,  GPIO_ODR_3,
        GPIO_ODR_4,  GPIO_ODR_5,  GPIO_ODR_6,  GPIO_ODR_7,
        GPIO_ODR_8,  GPIO_ODR_9,  GPIO_ODR_10, GPIO_ODR_11,
        GPIO_ODR_12, GPIO_ODR_13, GPIO_ODR_14, GPIO_ODR_15,
};

}  // namespace IO
