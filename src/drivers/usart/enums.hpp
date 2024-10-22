/**
 * @file enums.hpp
 * @brief Defines enumerations for various USART configuration parameters and status flags.
 *
 * This file contains enumeration types that represent settings for word length, oversampling mode,
 * parity, interrupts, DMA, ISR (Interrupt and Status Register) flags, ICR (Interrupt Clear Register) flags,
 * and baud rates used in configuring and controlling USART functionality.
 */

#pragma once
#include <cstdint>  // for uint8_t

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
