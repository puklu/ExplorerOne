#include "usart.hpp"
#include "common/assertHandler.hpp"
#include "common/AlternateFunctionsTable.hpp"
#include "drivers/io/pinBank.hpp"

namespace USART
{

void UsartInit(UsartInitStruct const &usart_init_struct)
{
    IO::GPIOpin *pin = usart_init_struct.pin;
    IO::eAlternateFunction alternate_function = usart_init_struct.alternate_function;
    eWordLength word_length = usart_init_struct.word_length;
    eOverSamplingMode oversampling_mode = usart_init_struct.oversampling_mode;
    eParityControlEnable parity_control = usart_init_struct.parity_control;
    eParitySelection parity_selection = usart_init_struct.parity_selection;
    eTxInterruptEnable tx_interrupt = usart_init_struct.tx_interrupt;
    eTxCompleteInterruptEnable tx_complete_interrupt = usart_init_struct.tx_complete_interrupt;
    eRxNotEmptyInterruptEnable rx_not_empty_interrupt = usart_init_struct.rx_not_empty_interrupt;
    eTxEnable tx_enable = usart_init_struct.tx_enable;
    eRxEnable rx_enable = usart_init_struct.rx_enable;
    eUsartEnable usart_enable = usart_init_struct.usart_enable;

    SelectUsart(pin, alternate_function);

    SetControlRegister(word_length, oversampling_mode, parity_control, parity_selection,
        tx_interrupt, tx_complete_interrupt, rx_not_empty_interrupt, tx_enable,
        rx_enable, usart_enable);

}

void SelectUsart(IO::GPIOpin *pin, IO::eAlternateFunction alternate_function)
{
    uint8_t port_num = pin->GetPortNumber();
    uint8_t pin_num = pin->GetPinNumber();

    // Make sure that the pin instance actually exists
    ASSERT(activePins[port_num][pin_num] != nullptr);
    
    pin->SetAlternateFunction(alternate_function);

    const void *selectedUsart;

    switch (port_num)
    {
    case 0:
        selectedUsart = aAltFunctionsAdressesPortA[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    case 1:
        selectedUsart = aAltFunctionsAdressesPortB[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    case 2:
        selectedUsart = aAltFunctionsAdressesPortC[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    case 3:
        selectedUsart = aAltFunctionsAdressesPortD[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    case 4:
        selectedUsart = aAltFunctionsAdressesPortE[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    case 5:
        selectedUsart = aAltFunctionsAdressesPortF[pin_num][static_cast<uint8_t>(alternate_function)];
        break;
    
    default:
        ASSERT(0);
        break;
    }

    if(selectedUsart != nullptr)
    {
            Usart = const_cast<USART_TypeDef*>(reinterpret_cast<const USART_TypeDef*>(selectedUsart));
    }
    else
    {
        ASSERT(0);
    }
}

void SetControlRegister(
    eWordLength &word_length,
    eOverSamplingMode &oversampling_mode,
    eParityControlEnable &parity_control,
    eParitySelection &parity_selection,
    eTxInterruptEnable &tx_interrupt,
    eTxCompleteInterruptEnable &tx_complete_interrupt,
    eRxNotEmptyInterruptEnable &rx_not_empty_interrupt,
    eTxEnable &tx_enable,
    eRxEnable &rx_enable,
    eUsartEnable &usart_enable)
{
    ASSERT(!(Usart->CR1 & 0x1));

    // Set word length
    if(word_length == eWordLength::USART_WORD_LEN_7BITS)
    {
        Usart->CR1 |= (1<<28); 
        Usart->CR1 &= ~(1<<12);
    }
    else if (word_length == eWordLength::USART_WORD_LEN_8BITS)
    {
        Usart->CR1 &= ~(1<<28); 
        Usart->CR1 &= ~(1<<12);
    }
    else if (word_length == eWordLength::USART_WORD_LEN_9BITS)
    {
        Usart->CR1 &= ~(1<<28); 
        Usart->CR1 |= (1<<12);
    } 

    // Set oversampling mode
    if(oversampling_mode == eOverSamplingMode::USART_OVERSAMPLING_BY_16)
    {
        Usart->CR1 &= ~(1<<15);
    }
    else if (oversampling_mode == eOverSamplingMode::USART_OVERSAMPLING_BY_8)
    {
        Usart->CR1 |= (1<<15);
    }  

    // Enable party control
    if(parity_control == eParityControlEnable::USART_PARITY_CONTROL_ENABLED)
    {
        Usart->CR1 |= (1<<10);
    }
    else if (parity_control == eParityControlEnable::USART_PARITY_CONTROL_DISABLED)
    {
        Usart->CR1 &= ~(1<<10);
    }  

    // Set parity selection
    ASSERT(Usart->CR1 & (1<<10)); // Assure that Parity control is enabled
    
    if(parity_selection == eParitySelection::USART_PARITY_EVEN)
    {
        Usart->CR1 &= ~(1<<9);
    }
    else if (parity_selection == eParitySelection::USART_PARITY_ODD)
    {
        Usart->CR1 |= (1<<9);
    }  
  

    // Enable TXE interrupt
    if(tx_interrupt == eTxInterruptEnable::USART_TX_INTERRUPT_ENABLE)
    {
        Usart->CR1 |= (1<<7);
    }
    else if (tx_interrupt == eTxInterruptEnable::USART_TX_INTERRUPT_DISABLE)
    {
        Usart->CR1 &= ~(1<<7);
    }  

    // Enable TC interrupt
    if(tx_complete_interrupt == eTxCompleteInterruptEnable::USART_TX_COMPLETE_INTERRUPT_ENABLE)
    {
        Usart->CR1 |= (1<<6);
    }
    else if (tx_complete_interrupt == eTxCompleteInterruptEnable::USART_TX_COMPLETE_INTERRUPT_DISABLE)
    {
        Usart->CR1 &= ~(1<<6);
    }  

    // Enable RXNE interrupt
    if(rx_not_empty_interrupt == eRxNotEmptyInterruptEnable::USART_RX_NOT_EMPTY_INTERRUPT_ENABLE)
    {
        Usart->CR1 |= (1<<5);
    }
    else if (rx_not_empty_interrupt == eRxNotEmptyInterruptEnable::USART_RX_NOT_EMPTY_INTERRUPT_DISABLE)
    {
        Usart->CR1 &= ~(1<<5);
    }  

    // Enable transmitter 
    if(tx_enable == eTxEnable::USART_TX_ENABLE)
    {
        Usart->CR1 |= (1<<3);
    }
    else if (tx_enable == eTxEnable::USART_TX_DISABLE)
    {
        Usart->CR1 &= ~(1<<3);
    }  

    // Enable receiver   
    if(rx_enable == eRxEnable::USART_RX_ENABLE)
    {
        Usart->CR1 |= (1<<2);
    }
    else if (rx_enable == eRxEnable::USART_RX_DISABLE)
    {
        Usart->CR1 &= ~(1<<2);
    }  


    // Enable/disable USART
    if(usart_enable == eUsartEnable::USART_ENABLE)
    {
        Usart->CR1 |= (1<<0);
    }
    else if (usart_enable == eUsartEnable::USART_DISABLE)
    {
        Usart->CR1 &= ~(1<<0);
    }

}

// void SetBaudRate(UsartInitStruct &usart_init_struct)
// {
//     ASSERT(!(Usart->CR1 & 0x1));

// }

// void SetGuardTimeAndPrescaler(UsartInitStruct &usart_init_struct)
// {

// }

// void SetReceiverTimeoutRegister(UsartInitStruct &usart_init_struct)
// {

// }

// void SetRequestRegister(UsartInitStruct &usart_init_struct)
// {

// }

uint32_t GetInterruptAndStatusRegister(eIsrFlags &flag)
{
    ASSERT(flag != eIsrFlags::RESERVED);
    return (Usart->ISR & (1<< static_cast<uint8_t>(flag)));
}

void SetInterruptClearFlagRegister(eIcrFlags &flag)
{
    if(flag != eIcrFlags::RESERVED5 && flag != eIcrFlags::RESERVED7 &&
            flag != eIcrFlags::RESERVED10 && flag != eIcrFlags::RESERVED13 &&
            flag != eIcrFlags::RESERVED14 && flag != eIcrFlags::RESERVED15 &&
            flag != eIcrFlags::RESERVED16 && flag != eIcrFlags::RESERVED17 &&
            flag != eIcrFlags::RESERVED18)
    {
        Usart->ICR  |= 1 << static_cast<uint8_t>(flag);
    }

    else
    {
        ASSERT(0);
    }
}

void ReceiveData(uintptr_t *data_buffer)
{
    // TODO: Something about various data sizes
    ASSERT(data_buffer != nullptr);
    *data_buffer = Usart->RDR;
}

void TransmitData(uintptr_t const *data)
{
    ASSERT(data != nullptr);
    Usart->TDR = *data;
}

} // namespace USART
