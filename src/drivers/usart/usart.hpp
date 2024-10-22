/**
 * @file usart.hpp
 * @brief Provides the USART namespace for configuring and using USART communication.
 *
 * This file contains the declaration of functions and structures for configuring USART
 * parameters, initializing the peripheral, and sending/receiving data using the USART module.
 */

#pragma once

#include "stm32f303xc.h"
#include "drivers/io/GPIOpin.hpp"
#include "enums.hpp"
#include "common/defines.hpp"


namespace USART
{

/**
 * @struct UsartInitStruct
 * @brief Represents the initialization parameters for the USART peripheral.
 *
 * This structure contains configuration settings such as baud rate, word length, 
 * parity control, interrupts, and transmission/reception settings for the USART module.
 */
typedef struct
{
    IO::GPIOpin *pin;
    IO::eAlternateFunction alternate_function;
    eWordLength word_length = eWordLength::USART_WORD_LEN_8BITS;
    eOverSamplingMode oversampling_mode = eOverSamplingMode::USART_OVERSAMPLING_BY_16;
    eParityControlEnable parity_control = eParityControlEnable::USART_PARITY_CONTROL_DISABLED;
    eParitySelection parity_selection = eParitySelection::USART_PARITY_NOT_SET;
    eTxInterruptEnable tx_interrupt = eTxInterruptEnable::USART_TX_INTERRUPT_ENABLE;
    eTxCompleteInterruptEnable tx_complete_interrupt = eTxCompleteInterruptEnable::USART_TX_COMPLETE_INTERRUPT_ENABLE;
    eRxNotEmptyInterruptEnable rx_not_empty_interrupt = eRxNotEmptyInterruptEnable::USART_RX_NOT_EMPTY_INTERRUPT_ENABLE;
    eTxEnable tx_enable = eTxEnable::USART_TX_ENABLE;
    eRxEnable rx_enable = eRxEnable::USART_RX_ENABLE;
    eUsartEnable usart_enable = eUsartEnable::USART_ENABLE;
    eBaudRate baud_rate = eBaudRate::USART_BAUD_RATE_115200;
} UsartInitStruct;


/**
 * @brief Pointer to the USART peripheral instance.
 */
extern USART_TypeDef* Usart;


/**
 * @brief Initializes the USART peripheral with the given configuration.
 * @param usart_init_struct The initialization structure with USART settings.
 */
void UsartInit(UsartInitStruct const &usart_init_struct);


/**
 * @brief Enables the clock for the USART peripheral.
 */
void EnableClock();


/**
 * @brief Configures the GPIO pin for USART communication with the specified alternate function.
 * @param pin The GPIO pin to be used for USART.
 * @param alternate_function The alternate function setting for the GPIO pin.
 */
void SelectUsart(IO::GPIOpin *pin, IO::eAlternateFunction alternate_function);


/**
 * @brief Configures the USART control register with the specified parameters.
 * @param word_length The word length setting.
 * @param oversampling_mode The oversampling mode.
 * @param parity_control The parity control enable setting.
 * @param parity_selection The parity selection.
 * @param tx_interrupt The transmit interrupt enable setting.
 * @param tx_complete_interrupt The transmit complete interrupt enable setting.
 * @param rx_not_empty_interrupt The receive not empty interrupt enable setting.
 * @param tx_enable The transmitter enable setting.
 * @param rx_enable The receiver enable setting.
 * @param usart_enable The USART enable setting.
 */
void SetControlRegister(eWordLength &word_length,
    eOverSamplingMode &oversampling_mode,
    eParityControlEnable &parity_control,
    eParitySelection &parity_selection,
    eTxInterruptEnable &tx_interrupt,
    eTxCompleteInterruptEnable &tx_complete_interrupt,
    eRxNotEmptyInterruptEnable &rx_not_empty_interrupt,
    eTxEnable &tx_enable,
    eRxEnable &rx_enable,
    eUsartEnable &usart_enable);


/**
 * @brief Sets the baud rate for USART communication.
 * @param baud_rate The desired baud rate setting.
 * @param oversampling_mode The oversampling mode to be used.
 */
void SetBaudRate(const eBaudRate &baud_rate, const eOverSamplingMode &oversampling_mode);

// void SetGuardTimeAndPrescaler(UsartInitStruct &usart_init_struct);

// void SetReceiverTimeoutRegister(UsartInitStruct &usart_init_struct);

// void SetRequestRegister(UsartInitStruct &usart_init_struct);


/**
 * @brief Retrieves the interrupt and status register value based on the specified flag.
 * @param flag The flag indicating which status to check.
 * @return The value of the interrupt and status register.
 */
uint32_t GetInterruptAndStatusRegister(eIsrFlags &flag);


/**
 * @brief Clears the interrupt flags based on the provided initialization structure.
 * @param usart_init_struct The USART initialization structure used to determine which flags to clear.
 */
void SetInterruptClearFlagRegister(UsartInitStruct &usart_init_struct);


/**
 * @brief Receives data from the USART peripheral.
 * @param data_buffer The buffer where the received data will be stored.
 */
void ReceiveData(uintptr_t *data_buffer);


/**
 * @brief Transmits a character via the USART peripheral.
 * @param data The character to be transmitted.
 */
void TransmitData(char const *data);

} // namespace USART
