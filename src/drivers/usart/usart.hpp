#pragma once

#include "stm32f303xc.h"
#include "drivers/io/GPIOpin.hpp"
#include "enums.hpp"
#include "common/defines.hpp"


namespace USART
{

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
} UsartInitStruct;

USART_TypeDef* Usart;

void UsartInit(UsartInitStruct const &usart_init_struct);

void SelectUsart(IO::GPIOpin *pin, IO::eAlternateFunction alternate_function);

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

// void SetBaudRate(UsartInitStruct &usart_init_struct);

// void SetGuardTimeAndPrescaler(UsartInitStruct &usart_init_struct);

// void SetReceiverTimeoutRegister(UsartInitStruct &usart_init_struct);

// void SetRequestRegister(UsartInitStruct &usart_init_struct);

uint32_t GetInterruptAndStatusRegister(eIsrFlags &flag);

void SetInterruptClearFlagRegister(UsartInitStruct &usart_init_struct);

void ReceiveData(uintptr_t *data_buffer);

void TransmitData(uintptr_t const *data);

} // namespace USART
