/**
 * @file UsartPin.hpp
 * @brief Provides the UsartPin class for handling USART pin configurations and data transmission.
 *
 * The UsartPin class manages USART-specific settings, such as baud rate, word length, parity,
 * and transmission modes, allowing interaction with USART hardware on the specified pin.
 */

#pragma once

#include <cstdint>
#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/IPin.hpp"
#include "drivers/interfaces/PinBase.hpp"


/**
 * @struct UsartPinInitStruct
 * @brief Initialization structure for configuring a USART pin.
 *
 * This structure extends PinBaseInitStruct, providing additional USART-specific configuration options.
 */
struct UsartPinInitStruct : public PinBaseInitStruct
{
    IO::eAlternateFunction alternate_function;
    USART::eWordLength word_length = USART::eWordLength::USART_WORD_LEN_8BITS;
    USART::eOverSamplingMode oversampling_mode = USART::eOverSamplingMode::USART_OVERSAMPLING_BY_16;
    USART::eParityControlEnable parity_control = USART::eParityControlEnable::USART_PARITY_CONTROL_DISABLED;
    USART::eParitySelection parity_selection = USART::eParitySelection::USART_PARITY_NOT_SET;
    USART::eTxEnable tx_enable = USART::eTxEnable::USART_TX_ENABLE;
    USART::eRxEnable rx_enable = USART::eRxEnable::USART_RX_ENABLE;
    USART::eUsartEnable usart_enable = USART::eUsartEnable::USART_ENABLE;
    USART::eBaudRate baud_rate = USART::eBaudRate::USART_BAUD_RATE_115200;
};


/**
 * @class UsartPin
 * @brief Class for configuring and managing a USART pin, including data transmission and reception.
 *
 * This class provides methods for configuring a USART pin and for transmitting and receiving
 * data over USART. UsartPin objects are constructed only through PinFactory, which accesses
 * its private constructor.
 */
class UsartPin : public PinBase
{
public:
    /**
     * @brief Receives data over USART.
     *
     * Reads incoming data from USART into the specified buffer.
     *
     * @param data_buffer Pointer to the buffer to store received data.
     */
    void ReceiveData(uintptr_t *const data_buffer) const;

    /**
     * @brief Transmits data over USART.
     *
     * Sends data over USART from the specified data buffer.
     *
     * @param data Pointer to the buffer containing data to be transmitted.
     */
    void TransmitData(char const *data);

private:
    
    /**
     * @brief Constructs a UsartPin with the specified settings.
     *
     * The constructor is private and accessible only by PinFactory.
     *
     * @param pin_init_struct Initialization structure with USART settings.
     */
    explicit UsartPin(UsartPinInitStruct const &pin_init_struct);

    friend class PinFactory; // Allows PinFactory to access private constructor

    /**
     * @brief Configures the mode for the USART pin.
     *
     * Sets the mode of the pin according to the UsartPinInitStruct.
     */
    void SetMode();

    /**
     * @brief Sets the alternate function for the USART pin.
     *
     * Configures the alternate function setting for USART on this pin.
     */
    void SetAlternateFunction();

    /**
     * @brief Enables the clock for USART peripheral.
     *
     * This function enables the peripheral clock for the USART to start communication.
     */
    void EnableClock() const;

    /**
     * @brief Selects the USART instance for this pin.
     *
     * This function associates the USART instance with this pin based on the configuration.
     */
    void SelectUsart();

    /**
     * @brief Configures the USART control register.
     *
     * Sets various control settings, including word length, parity, and enabling the USART.
     */
    void SetControlRegister();

    /**
     * @brief Sets the baud rate for the USART.
     *
     * Configures the baud rate for communication as per the initialization structure.
     */
    void SetBaudRate();


    // void SetGuardTimeAndPrescaler();
    // void SetReceiverTimeoutRegister();
    // void SetRequestRegister();


    /**
     * @brief Retrieves the interrupt and status register for USART.
     *
     * @param flag ISR flag to retrieve.
     * @return The value of the requested ISR flag.
     */
    uint32_t GetInterruptAndStatusRegister(USART::eIsrFlags const &flag) const;

    /**
     * @brief Clears the specified interrupt flag in the status register.
     *
     * @param flag ISR flag to clear.
     */
    void SetInterruptClearFlagRegister(USART::eIcrFlags const &flag);

    /**
     * @brief Enables the TX register empty interrupt.
     */
    void EnableTxRegisterEmptyInterrupt();

    /**
     * @brief Disables the TX register empty interrupt.
     */
    void DisableTxRegisterEmptyInterrupt();

    /**
     * @brief Enables the TX complete interrupt.
     */
    void EnableTxCompleteInterrupt();

    /**
     * @brief Disables the TX complete interrupt.
     */
    void DisableTxCompleteInterrupt();

    /**
     * @brief Enables the RX not empty interrupt.
     */
    void EnableRxNotEmptyInterrupt();

    /**
     * @brief Disables the RX not empty interrupt.
     */
    void DisableRxNotEmptyInterrupt();

    /**
     * @brief Enables all configured USART interrupts.
     */
    void EnableInterrupts();

    /**
     * @brief Enables the NVIC for USART handling.
     *
     * Configures the Nested Vectored Interrupt Controller (NVIC) for USART interrupts.
     */
    void EnableNVIC();

    /**
     * @brief Retrieves the IRQ number associated with this USART pin.
     */
    void GetIRQn();


    USART_TypeDef*                 mpUsart = nullptr;
    IO::eAlternateFunction         mAlternateFunction;
    USART::eWordLength             mWordLength = USART::eWordLength::USART_WORD_LEN_8BITS;
    USART::eOverSamplingMode       mOversamplingMode = USART::eOverSamplingMode::USART_OVERSAMPLING_BY_16;
    USART::eParityControlEnable    mParityControl = USART::eParityControlEnable::USART_PARITY_CONTROL_DISABLED;
    USART::eParitySelection        mParitySelection = USART::eParitySelection::USART_PARITY_NOT_SET;
    USART::eTxEnable               mTxEnable = USART::eTxEnable::USART_TX_ENABLE;
    USART::eRxEnable               mRxEnable = USART::eRxEnable::USART_RX_ENABLE;
    USART::eUsartEnable            mUsartEnable = USART::eUsartEnable::USART_ENABLE;
    USART::eBaudRate               mBaudRate = USART::eBaudRate::USART_BAUD_RATE_115200;
    IO::eMode                      mMode = IO::eMode::IO_MODE_ALT_FUNCTION;
    IRQn_Type                      mIrqNumber;
};
