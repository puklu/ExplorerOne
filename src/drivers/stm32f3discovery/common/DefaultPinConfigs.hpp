#pragma once

#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "drivers/stm32f3discovery/usart/UsartPin.hpp"

struct DefaultPinConfigs
{
    inline static constexpr GpioPinInitStruct Ld4Blue_Output_PushPull_PullDown{
        IO::ePin::IO_TEST_LED_LD4_BLUE,
        IO::eMode::IO_MODE_OUTPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct FrontMotorRightA_Alt_PullDown{
        IO::ePin::IO_FRONT_MOTOR_RIGHT_A,
        IO::eMode::IO_MODE_ALT_FUNCTION,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct FrontMotorRightB_Output_PullDown{
        IO::ePin::IO_FRONT_MOTOR_RIGHT_B,
        IO::eMode::IO_MODE_OUTPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct FrontMotorLeftA_Alt_PullDown{
        IO::ePin::IO_FRONT_MOTOR_LEFT_A,
        IO::eMode::IO_MODE_ALT_FUNCTION,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct FrontMotorLeftB_Output_PullDown{
        IO::ePin::IO_FRONT_MOTOR_LEFT_B,
        IO::eMode::IO_MODE_OUTPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct BackMotorRightA_Alt_PullDown{
        IO::ePin::IO_BACK_MOTOR_RIGHT_A,
        IO::eMode::IO_MODE_ALT_FUNCTION,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct BackMotorRightB_Output_PullDown{
        IO::ePin::IO_BACK_MOTOR_RIGHT_B,
        IO::eMode::IO_MODE_OUTPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct BackMotorLeftA_Alt_PullDown{
        IO::ePin::IO_BACK_MOTOR_LEFT_A,
        IO::eMode::IO_MODE_ALT_FUNCTION,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct BackMotorLeftB_Output_PullDown{
        IO::ePin::IO_BACK_MOTOR_LEFT_B,
        IO::eMode::IO_MODE_OUTPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr GpioPinInitStruct
        Ld5Orange_Output_PushPull_PullDown{
            IO::ePin::IO_TEST_LED_LD5_ORANGE,
            IO::eMode::IO_MODE_OUTPUT,
            IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL,
            IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
            IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
        };

    inline static constexpr GpioPinInitStruct D0_Unused_Input_PushPull_PullDown{
        IO::ePin::IO_UNUSED_D0,
        IO::eMode::IO_MODE_INPUT,
        IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET,
        IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET,
        IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    inline static constexpr UsartPinInitStruct Uart4_Tx_Af5_115200{
        IO::ePin::IO_UART4_TX_PRINT,
        IO::eAlternateFunction::IO_AF5,
        USART::eWordLength::USART_WORD_LEN_8BITS,
        USART::eOverSamplingMode::USART_OVERSAMPLING_BY_16,
        USART::eParityControlEnable::USART_PARITY_CONTROL_DISABLED,
        USART::eParitySelection::USART_PARITY_NOT_SET,
        USART::eTxEnable::USART_TX_ENABLE,
        USART::eRxEnable::USART_RX_ENABLE,
        USART::eUsartEnable::USART_ENABLE,
        USART::eBaudRate::USART_BAUD_RATE_115200,
    };
};
