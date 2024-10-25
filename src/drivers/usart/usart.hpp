
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
    eTxEnable tx_enable = eTxEnable::USART_TX_ENABLE;
    eRxEnable rx_enable = eRxEnable::USART_RX_ENABLE;
    eUsartEnable usart_enable = eUsartEnable::USART_ENABLE;
    eBaudRate baud_rate = eBaudRate::USART_BAUD_RATE_115200;
} UsartInitStruct;

class Usart
{
    public:
        explicit Usart(UsartInitStruct const &usart_init_struct);
        void ReceiveData(uintptr_t *const data_buffer) const;
        void TransmitData(char const *data);

    private:
        void EnableClock() const;
        void SelectUsart();
        void SetControlRegister();
        void SetBaudRate();
        // void SetGuardTimeAndPrescaler();
        // void SetReceiverTimeoutRegister();
        // void SetRequestRegister();
        uint32_t GetInterruptAndStatusRegister(eIsrFlags &flag) const;
        void SetInterruptClearFlagRegister(eIcrFlags &flag);
        void EnableTxRegisterEmptyInterrupt();
        void DisableTxRegisterEmptyInterrupt();
        void EnableTxCompleteInterrupt();
        void DisableTxCompleteInterrupt();
        void EnableRxNotEmptyInterrupt();
        void DisableRxNotEmptyInterrupt();
        void EnableInterrupts() const;
        IRQn_Type GetIRQn() const;

        IO::GPIOpin *mpPin;
        USART_TypeDef* mpUsart;
        IO::eAlternateFunction mAlternateFunction;
        eWordLength mWordLength = eWordLength::USART_WORD_LEN_8BITS;
        eOverSamplingMode mOversamplingMode = eOverSamplingMode::USART_OVERSAMPLING_BY_16;
        eParityControlEnable mParityControl = eParityControlEnable::USART_PARITY_CONTROL_DISABLED;
        eParitySelection mParitySelection = eParitySelection::USART_PARITY_NOT_SET;
        eTxEnable mTxEnable = eTxEnable::USART_TX_ENABLE;
        eRxEnable mRxEnable = eRxEnable::USART_RX_ENABLE;
        eUsartEnable mUsartEnable = eUsartEnable::USART_ENABLE;
        eBaudRate mBaudRate = eBaudRate::USART_BAUD_RATE_115200;

};    

} // namespace USART
