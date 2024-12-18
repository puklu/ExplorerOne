/**
 * @file PinDefinitions.hpp
 * @brief Enumeration definitions for GPIO and IO functionalities.
 *
 * This file contains the enumerations used to represent various aspects
 * of pins and their configurations.
 *
 */

#pragma once
#include <cstdint>  // for uint8_t

/**
 * @namespace IO
 * @brief All IO related enums should fall under this namespace.
 */
namespace IO
{

/**
 * @enum
 * @brief The purpose of this enum is to extract port number and pin number
 * from an enum. There are 6(ports) times 16(pin in each port) numbers in this
 * enum.
 * @details The number represented by PB1 is 17, which is 0001 0001
 *          Since there are 16 pins per port, we need 4 bits. The last
 *          4 bits can be used to extract pin number. And there are 6
 *          ports so the next 3 bits can be used to extract port.
 *
 *          PB1 = 0001 0001
 *          pin = PB1 & 0x0F = 0000 0001, which is 1 (pin indexing starts from
 * 0) here 0X0F is PIN_MASK
 *
 *          port = (PB1 & 0X70) >> 4 = 0000 0001, which is 1 (port indexing
 * starts from 0) here 0x70 is PORT_MASK, and 4 is PORT_OFFSET
 *
 */
typedef enum : uint8_t
{
    // clang-format off
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
    // clang-format on
} eGenericPort;

/**
 * @enum
 * @brief Enum for the user-friendly names of each pin on the board.
 */
typedef enum : uint8_t
{
    IO_FRONT_MOTOR_RIGHT_A          = PA0,
    IO_FRONT_MOTOR_RIGHT_B          = PA1,
    IO_FRONT_MOTOR_LEFT_A           = PA2,
    IO_FRONT_MOTOR_LEFT_B           = PA3,
    IO_BACK_MOTOR_RIGHT_A           = PA4,
    IO_BACK_MOTOR_RIGHT_B           = PA5,
    IO_BACK_MOTOR_LEFT_A            = PA6,
    IO_BACK_MOTOR_LEFT_B            = PA7,
    IO_FRONT_ULTRASONIC_SENSOR_TRIG = PA8,
    IO_FRONT_ULTRASONIC_SENSOR_ECHO = PA9,
    IO_BACK_ULTRASONIC_SENSOR_TRIG  = PA10,
    IO_BACK_ULTRASONIC_SENSOR_ECHO  = PA11,
    IO_UNUSED_A12                   = PA12,
    IO_UNUSED_A13                   = PA13,
    IO_UNUSED_A14                   = PA14,
    IO_UNUSED_A15                   = PA15,
    IO_UNUSED_B0                    = PB0,
    IO_UNUSED_B1                    = PB1,
    IO_UNUSED_B2                    = PB2,
    IO_UNUSED_B3                    = PB3,
    IO_UNUSED_B4                    = PB4,
    IO_UNUSED_B5                    = PB5,
    IO_UNUSED_B6                    = PB6,
    IO_UNUSED_B7                    = PB7,
    IO_UNUSED_B8                    = PB8,
    IO_UNUSED_B9                    = PB9,
    IO_UNUSED_B10                   = PB10,
    IO_UNUSED_B11                   = PB11,
    IO_UNUSED_B12                   = PB12,
    IO_UNUSED_B13                   = PB13,
    IO_UNUSED_B14                   = PB14,
    IO_UNUSED_B15                   = PB15,
    IO_UNUSED_C0                    = PC0,
    IO_UNUSED_C1                    = PC1,
    IO_UNUSED_C2                    = PC2,
    IO_UNUSED_C3                    = PC3,
    IO_UNUSED_C4                    = PC4,
    IO_UNUSED_C5                    = PC5,
    IO_UNUSED_C6                    = PC6,
    IO_UNUSED_C7                    = PC7,
    IO_UNUSED_C8                    = PC8,
    IO_UNUSED_C9                    = PC9,
    IO_UART4_TX_PRINT               = PC10,
    IO_UART4_RX                     = PC11,
    IO_UNUSED_C12                   = PC12,
    IO_UNUSED_C13                   = PC13,
    IO_UNUSED_C14                   = PC14,
    IO_UNUSED_C15                   = PC15,
    IO_UNUSED_D0                    = PD0,
    IO_UNUSED_D1                    = PD1,
    IO_UNUSED_D2                    = PD2,
    IO_UNUSED_D3                    = PD3,
    IO_UNUSED_D4                    = PD4,
    IO_UNUSED_D5                    = PD5,
    IO_UNUSED_D6                    = PD6,
    IO_UNUSED_D7                    = PD7,
    IO_UNUSED_D8                    = PD8,
    IO_UNUSED_D9                    = PD9,
    IO_UNUSED_D10                   = PD10,
    IO_UNUSED_D11                   = PD11,
    IO_UNUSED_D12                   = PD12,
    IO_UNUSED_D13                   = PD13,
    IO_UNUSED_D14                   = PD14,
    IO_UNUSED_D15                   = PD15,
    IO_UNUSED_E0                    = PE0,
    IO_UNUSED_E1                    = PE1,
    IO_UNUSED_E2                    = PE2,
    IO_UNUSED_E3                    = PE3,
    IO_UNUSED_E4                    = PE4,
    IO_UNUSED_E5                    = PE5,
    IO_UNUSED_E6                    = PE6,
    IO_UNUSED_E7                    = PE7,
    IO_TEST_LED_LD4_BLUE            = PE8,
    IO_TEST_LED_LD3_RED             = PE9,
    IO_TEST_LED_LD5_ORANGE          = PE10,
    IO_TEST_LED_LD7_GREEN           = PE11,
    IO_TEST_LED_LD9_BLUE            = PE12,
    IO_TEST_LED_LD10_RED            = PE13,
    IO_TEST_LED_LD8_ORANGE          = PE14,
    IO_TEST_LED_LD6_GREEN           = PE15,
    IO_UNUSED_F0                    = PF0,
    IO_UNUSED_F1                    = PF1,
    IO_UNUSED_F2                    = PF2,
    IO_UNUSED_F3                    = PF3,
    IO_UNUSED_F4                    = PF4,
    IO_UNUSED_F5                    = PF5,
    IO_UNUSED_F6                    = PF6,
    IO_UNUSED_F7                    = PF7,
    IO_UNUSED_F8                    = PF8,
    IO_UNUSED_F9                    = PF9,
    IO_UNUSED_F10                   = PF10,
    IO_UNUSED_F11                   = PF11,
    IO_UNUSED_F12                   = PF12,
    IO_UNUSED_F13                   = PF13,
    IO_UNUSED_F14                   = PF14,
    IO_UNUSED_F15                   = PF15,
} ePin;

/**
 * @enum
 * @brief Enum for direction of an IO pin.
 */
enum class eMode : uint8_t
{
    IO_MODE_INPUT,
    IO_MODE_OUTPUT,
    IO_MODE_ALT_FUNCTION,
    IO_MODE_ANALOG,
    IO_MODE_NOT_SET,
};

/**
 * @enum
 * @brief Enum for alternate functions of an IO pin.
 */
enum class eAlternateFunction : uint8_t
{
    IO_AF0,
    IO_AF1,
    IO_AF2,
    IO_AF3,
    IO_AF4,
    IO_AF5,
    IO_AF6,
    IO_AF7,
    IO_AF8,
    IO_AF9,
    IO_AF10,
    IO_AF11,
    IO_AF12,
    IO_AF13,
    IO_AF14,
    IO_AF15,
};

/**
 * @enum
 * @brief Enum for output type of an IO pin.
 */
enum class eOutputType : uint8_t
{
    IO_OUTPUT_TYPE_PUSH_PULL,
    IO_OUTPUT_TYPE_OPEN_DRAIN,
    IO_OUTPUT_TYPE_NOT_SET,
};

/**
 * @enum
 * @brief Enum for output speed of an IO pin.
 */
enum class eOutputSpeed : uint8_t
{
    IO_OUTPUT_LOW_SPEED,
    IO_OUTPUT_MEDIUM_SPEED,
    IO_OUTPUT_HIGH_SPEED,
    IO_OUTPUT_SPEED_NOT_SET,
};

/**
 * @enum
 * @brief Enum for pull-up/pull-down resistor direction of an IO pin.
 */
enum class ePupdResistor : uint8_t
{
    IO_RESISTOR_NO_PUPD,
    IO_RESISTOR_PULL_UP,
    IO_RESISTOR_PULL_DOWN,
    IO_RESISTOR_RESERVED,
    IO_RESISTOR_NOT_SET,
};

/**
 * @enum
 * @brief Enum for digital values at an IO pin, i.e low or high.
 */
typedef enum : uint32_t
{
    IO_VALUE_LOW,
    IO_VALUE_HIGH,
    IO_VALUE_UNKNOWN,
} eValue;

typedef enum : uint8_t
{
    IO_INTERRUPT_TRIGGER_RISING_EDGE,
    IO_INTERRUPT_TRIGGER_FALLING_EDGE,
    IO_INTERRUPT_TRIGGER_BOTH_EDGE,
    IO_INTERRUPT_TRIGGER_NOT_SET,
} eTriggerEdge;


enum class ePinType : uint8_t
{
    IO_PIN_TYPE_GPIO,
    IO_PIN_TYPE_EXTI,
    IO_PIN_TYPE_USART,
    IO_PIN_TYPE_PRINTING_USART,
};

}  // namespace IO

/**
 * @namespace USART
 * @brief All USART related enums should fall under this namespace
 */
namespace USART
{

/**
 * @enum eWordLength
 * @brief Defines possible word length configurations for USART communication.
 */
enum class eWordLength : uint8_t
{
    USART_WORD_LEN_8BITS,
    USART_WORD_LEN_9BITS,
    USART_WORD_LEN_7BITS,
    USART_WORD_LEN_NOT_SET,
};


/**
 * @enum eOverSamplingMode
 * @brief Defines USART oversampling modes.
 */
enum class eOverSamplingMode : uint8_t
{
    USART_OVERSAMPLING_BY_16,
    USART_OVERSAMPLING_BY_8,
    USART_OVERSAMPLING_NOT_SET,
};


/**
 * @enum eParityControlEnable
 * @brief Defines parity control enable settings for USART.
 */
enum class eParityControlEnable : uint8_t
{
    USART_PARITY_CONTROL_DISABLED,
    USART_PARITY_CONTROL_ENABLED,
    USART_PARITY_CONTROL_NOT_SET,
};


/**
 * @enum eParitySelection
 * @brief Defines USART parity selection options.
 */
enum class eParitySelection : uint8_t
{
    USART_PARITY_EVEN,
    USART_PARITY_ODD,
    USART_PARITY_NOT_SET,
};


/**
 * @enum eTxInterruptEnable
 * @brief Defines transmit interrupt enable settings for USART.
 */
enum class eTxInterruptEnable : uint8_t
{
    USART_TX_INTERRUPT_DISABLE,
    USART_TX_INTERRUPT_ENABLE,
    USART_TX_INTERRUPT_NOT_SET,
};


/**
 * @enum eTxCompleteInterruptEnable
 * @brief Defines transmit complete interrupt enable settings for USART.
 */
enum class eTxCompleteInterruptEnable : uint8_t
{
    USART_TX_COMPLETE_INTERRUPT_DISABLE,
    USART_TX_COMPLETE_INTERRUPT_ENABLE,
    USART_TX_COMPLETE_INTERRUPT_NOT_SET,
};


/**
 * @enum eRxNotEmptyInterruptEnable
 * @brief Defines receive not empty interrupt enable settings for USART.
 */
enum class eRxNotEmptyInterruptEnable : uint8_t
{
    USART_RX_NOT_EMPTY_INTERRUPT_DISABLE,
    USART_RX_NOT_EMPTY_INTERRUPT_ENABLE,
    USART_RX_NOT_EMPTY_INTERRUPT_NOT_SET,
};


/**
 * @enum eTxEnable
 * @brief Defines settings for enabling or disabling the transmitter.
 */
enum class eTxEnable : uint8_t  
{
    USART_TX_DISABLE,
    USART_TX_ENABLE,
};


/**
 * @enum eRxEnable
 * @brief Defines settings for enabling or disabling the receiver.
 */
enum class eRxEnable : uint8_t  
{
    USART_RX_DISABLE,
    USART_RX_ENABLE,
};


/**
 * @enum eUsartEnable
 * @brief Defines settings for enabling or disabling the USART.
 */
enum class eUsartEnable : uint8_t  
{
    USART_DISABLE,
    USART_ENABLE,
};


/**
 * @enum eDmaTxEnable
 * @brief Defines settings for enabling or disabling DMA for transmission.
 */
enum class eDmaTxEnable : uint8_t  
{
    USART_DMA_TX_ENABLE,
    USART_DMA_TX_DISABLE,
    USART_DMA_TX_NOT_SET,
};


/**
 * @enum eDmaRxEnable
 * @brief Defines settings for enabling or disabling DMA for reception.
 */
enum class eDmaRxEnable : uint8_t  
{
    USART_DMA_RX_ENABLE,
    USART_DMA_RX_DISABLE,
    USART_DMA_RX_NOT_SET,
};


/**
 * @enum eIsrFlags
 * @brief Defines flags for the USART Interrupt and Status Register (ISR).
 */
enum class eIsrFlags : uint8_t  
{
    USART_ISR_PARITY_ERROR,
    USART_ISR_FRAMING_ERROR,
    USART_ISR_NOISE_DETECTION,
    USART_ISR_OVERRUN_ERROR,
    USART_ISR_IDLE_LINE_DETECTED,
    USART_ISR_READ_DATA_REGISTER_NOT_EMPTY,
    USART_ISR_TRANSMISSION_COMPLETE,
    USART_ISR_TRANSMIT_DATA_REGISTER_EMPTY,
    USART_ISR_LIN_BREAK_DETECTION,
    USART_ISR_CTS_INTERRUPT,
    USART_ISR_CTS_FLAG,
    USART_ISR_RECEIVER_TIMEOUT,
    USART_ISR_END_OF_BLOCK,
    RESERVED,
    USART_ISR_AUTO_BAUD_RATE_ERROR,
    USART_ISR_AUTO_BAUD_RATE_FLAG,
    USART_ISR_BUSY_FLAG,
    USART_ISR_CHARACTER_MATCH_FLAG,
    USART_ISR_SEND_BREAK_FLAG,
    USART_ISR_RECEIVER_WAKUP_FROM_MUTE_MODE,
    USART_ISR_WAKEUP_FROM_STOP_MODE_FLAG,
    USART_ISR_TRANSMIT_ENABLE_ACK_FLAG,
    USART_ISR_RECEIVE_ENABLE_ACK_FLAG,
};


/**
 * @enum eIcrFlags
 * @brief Defines flags for the USART Interrupt Clear Register (ICR).
 */
enum class eIcrFlags : uint8_t  
{
    USART_ICR_PARITY_ERROR_CLEAR_FLAG,
    USART_ICR_FRAMING_ERROR_CLEAR_FLAG,
    USART_ICR_NOISE_DETECTED_CLEAR_FLAG,
    USART_ICR_OVERRUN_ERROR_CLEAR_FLAG,
    USART_ICR_IDLE_LINE_DETECTED_CLEAR_FLAG,
    RESERVED5,
    USART_ICR_TRANSMISSION_COMPLETE_CLEAR_FLAG,
    RESERVED7,
    USART_ICR_LIN_BREAK_DETECTION_CLEAR_FLAG,
    USART_ICR_CTS_CLEAR_FLAG,
    RESERVED10,
    USART_ICR_RECEIVER_TIMEOUT_CLEAR_FLAG,
    USART_ICR_END_OF_BLOCK_CLEAR_FLAG,
    RESERVED13,
    RESERVED14,
    RESERVED15,
    RESERVED16,
    USART_ICR_CHARACTER_MATCH_CLEAR_FLAG,
    RESERVED17,
    RESERVED18,
    USART_ICR_WAKEUP_FROM_STOP_MODE_CLEAR_FLAG,
};


/**
 * @enum eBaudRate
 * @brief Defines possible baud rate settings for USART communication.
 */
enum class eBaudRate : uintptr_t
{
    USART_BAUD_RATE_9600,
    USART_BAUD_RATE_19200,
    USART_BAUD_RATE_38400,
    USART_BAUD_RATE_57600,
    USART_BAUD_RATE_115200,
};

} // namespace USART
