#include "usart.hpp"
#include "common/assertHandler.hpp"
#include "common/AlternateFunctionsTable.hpp"
#include "drivers/io/pinBank.hpp"
#include "common/registerArrays.hpp"

namespace USART
{

Usart::Usart(UsartInitStruct const &usart_init_struct):
    mpPin(usart_init_struct.pin),
    mAlternateFunction(usart_init_struct.alternate_function),
    mWordLength(usart_init_struct.word_length),
    mOversamplingMode(usart_init_struct.oversampling_mode),
    mParityControl(usart_init_struct.parity_control),
    mParitySelection(usart_init_struct.parity_selection),
    mTxEnable(usart_init_struct.tx_enable),
    mRxEnable(usart_init_struct.rx_enable),
    mUsartEnable(usart_init_struct.usart_enable),
    mBaudRate(usart_init_struct.baud_rate)
{
    SelectUsart();
    EnableClock();
    EnableInterrupts();
    SetBaudRate();
    SetControlRegister();
}

void Usart::ReceiveData(uintptr_t *const data_buffer) const
{
    // TODO: Something about various data sizes
    ASSERT(data_buffer != nullptr);

    // Wait until Read data register is not empty
    while(!(mpUsart->ISR & USART_ISR_RXNE));

    *data_buffer = mpUsart->RDR;
}

void Usart::TransmitData(char const *data)
{
    ASSERT(data != nullptr);

    // EnableTxRegisterEmptyInterrupt();

    while(!(mpUsart->ISR & USART_ISR_TXE));
    
    mpUsart->TDR = *data;

}

void Usart::EnableClock() const
{
       if(mpUsart == USART1){
        RCC->APB2ENR |= IO::aUsartEnableRegistersMasks[0]; // USART1 (1<<14);
    }

    else if (mpUsart == USART2)
    {
       RCC->APB1ENR |= IO::aUsartEnableRegistersMasks[1]; // (1<<17);
    }

    else if (mpUsart == USART3)
    {
        RCC->APB1ENR |= IO::aUsartEnableRegistersMasks[2]; // (1<<18);
    }

    else if (mpUsart == UART4)
    {
        RCC->APB1ENR |= IO::aUsartEnableRegistersMasks[3]; // (1<<19);
    }

    else if (mpUsart == UART5)
    {
        RCC->APB1ENR |= IO::aUsartEnableRegistersMasks[4]; // (1<<20);
    } 
}

void Usart::SelectUsart()
{
    uint8_t port_num = mpPin->GetPortNumber();
    uint8_t pin_num = mpPin->GetPinNumber();

    // Make sure that the pin instance actually exists
    ASSERT(activePins[port_num][pin_num] != nullptr);
    
    mpPin->SetAlternateFunction(mAlternateFunction);

    const void *selectedUsart;

    switch (port_num)
    {
    case 0:
        selectedUsart = aAltFunctionsAdressesPortA[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 1:
        selectedUsart = aAltFunctionsAdressesPortB[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 2:
        selectedUsart = aAltFunctionsAdressesPortC[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 3:
        selectedUsart = aAltFunctionsAdressesPortD[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 4:
        selectedUsart = aAltFunctionsAdressesPortE[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 5:
        selectedUsart = aAltFunctionsAdressesPortF[pin_num][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    default:
        ASSERT(0);
        break;
    }

    if(selectedUsart != nullptr)
    {
            mpUsart = const_cast<USART_TypeDef*>(reinterpret_cast<const USART_TypeDef*>(selectedUsart));
    }
    else
    {
        ASSERT(0);
    }
}

void Usart::SetControlRegister()
{
       ASSERT(!(mpUsart->CR1 & 0x1));

    // Set word length
    if(mWordLength == eWordLength::USART_WORD_LEN_7BITS)
    {
        mpUsart->CR1 |= (1<<28); 
        mpUsart->CR1 &= ~(1<<12);
    }
    else if (mWordLength == eWordLength::USART_WORD_LEN_8BITS)
    {
        mpUsart->CR1 &= ~(1<<28); 
        mpUsart->CR1 &= ~(1<<12);
    }
    else if (mWordLength == eWordLength::USART_WORD_LEN_9BITS)
    {
        mpUsart->CR1 &= ~(1<<28); 
        mpUsart->CR1 |= (1<<12);
    } 

    // Set oversampling mode
    if(mOversamplingMode == eOverSamplingMode::USART_OVERSAMPLING_BY_16)
    {
        mpUsart->CR1 &= ~(1<<15);
    }
    else if (mOversamplingMode == eOverSamplingMode::USART_OVERSAMPLING_BY_8)
    {
        mpUsart->CR1 |= (1<<15);
    }  

    // Enable party control
    if(mParityControl == eParityControlEnable::USART_PARITY_CONTROL_ENABLED)
    {
        mpUsart->CR1 |= (1<<10);
    }
    else if (mParityControl == eParityControlEnable::USART_PARITY_CONTROL_DISABLED)
    {
        mpUsart->CR1 &= ~(1<<10);
    }

    // Set parity selection
    if(mpUsart->CR1 & (1<<10)) // Assure that Parity control is enabled
    {
        if(mParitySelection == eParitySelection::USART_PARITY_EVEN)
        {
            mpUsart->CR1 &= ~(1<<9);
        }
        else if (mParitySelection == eParitySelection::USART_PARITY_ODD)
        {
            mpUsart->CR1 |= (1<<9);
        }
    }
  
    // Enable transmitter 
    if(mTxEnable == eTxEnable::USART_TX_ENABLE)
    {
        mpUsart->CR1 |= (1<<3);
    }
    else if (mTxEnable == eTxEnable::USART_TX_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<3);
    }  

    // Enable receiver   
    if(mRxEnable == eRxEnable::USART_RX_ENABLE)
    {
        mpUsart->CR1 |= (1<<2);
    }
    else if (mRxEnable == eRxEnable::USART_RX_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<2);
    }  


    // Enable/disable USART
    if(mUsartEnable == eUsartEnable::USART_ENABLE)
    {
        mpUsart->CR1 |= (1<<0);
    }
    else if (mUsartEnable == eUsartEnable::USART_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<0);
    }
}

void Usart::SetBaudRate()
{
    ASSERT(!(mpUsart->CR1 & 0x1));

    // TODO: This should change with the selected clock. Hardcoded for now.
    uintptr_t clock = SYS_CLK;
    uintptr_t desiredBaudRate = 0;

    switch (mBaudRate)
    {
    case eBaudRate::USART_BAUD_RATE_9600:
        desiredBaudRate = 9600;
        break;

    case eBaudRate::USART_BAUD_RATE_57600:
        desiredBaudRate = 57600;
    break;

    case eBaudRate::USART_BAUD_RATE_38400:
        desiredBaudRate = 38400;
    break;

    case eBaudRate::USART_BAUD_RATE_19200:
        desiredBaudRate = 19200;
    break;

    case eBaudRate::USART_BAUD_RATE_115200:
        desiredBaudRate = 115200;
    break;

    default:
        ASSERT(0);
        return;
    }

    uintptr_t UsartDiv = 0;

    if (mOversamplingMode == eOverSamplingMode::USART_OVERSAMPLING_BY_16)
    {
        UsartDiv = clock/desiredBaudRate;
        mpUsart->BRR |= UsartDiv;
    }
    else if (mOversamplingMode == eOverSamplingMode::USART_OVERSAMPLING_BY_8)
    {
        UsartDiv = (2 * clock)/desiredBaudRate;
        mpUsart->BRR  = (UsartDiv & USART_BRR_DIV_FRACTION) >> 1;
        mpUsart->BRR |= (UsartDiv & USART_BRR_DIV_MANTISSA);
    }
    else
    {
        ASSERT(0);
        return;
    }
}

// void Usart::SetGuardTimeAndPrescaler(){}
// void Usart::SetReceiverTimeoutRegister(){}
// void Usart::SetRequestRegister(){}

uint32_t Usart::GetInterruptAndStatusRegister(eIsrFlags &flag) const
{
    ASSERT(flag != eIsrFlags::RESERVED);
    return (mpUsart->ISR & (1<< static_cast<uint8_t>(flag)));
}

void Usart::SetInterruptClearFlagRegister(eIcrFlags &flag)
{
    if(flag != eIcrFlags::RESERVED5 && flag != eIcrFlags::RESERVED7 &&
       flag != eIcrFlags::RESERVED10 && flag != eIcrFlags::RESERVED13 &&
       flag != eIcrFlags::RESERVED14 && flag != eIcrFlags::RESERVED15 &&
       flag != eIcrFlags::RESERVED16 && flag != eIcrFlags::RESERVED17 &&
       flag != eIcrFlags::RESERVED18)
    {
        mpUsart->ICR  |= 1 << static_cast<uint8_t>(flag);
    }

    else
    {
        ASSERT(0);
    }
}

void Usart::EnableTxRegisterEmptyInterrupt()
{
    mpUsart->CR1 |=  USART_CR1_TXEIE;
}

void Usart::DisableTxRegisterEmptyInterrupt()
{
    mpUsart->CR1 &= ~USART_CR1_TXEIE;
}

void Usart::EnableTxCompleteInterrupt()
{
    mpUsart->CR1 |= USART_CR1_TCIE;    
}

void Usart::DisableTxCompleteInterrupt()
{
    mpUsart->CR1 &= ~USART_CR1_TCIE;
}

void Usart::EnableRxNotEmptyInterrupt()
{
    mpUsart->CR1 |= USART_CR1_RXNEIE;
}

void Usart::DisableRxNotEmptyInterrupt()
{
    mpUsart->CR1 &= ~USART_CR1_RXNEIE;
}

void Usart::EnableInterrupts() const
{
    IRQn_Type irqNum = GetIRQn();
    NVIC_EnableIRQ(irqNum);
    NVIC_SetPriority(irqNum, 1);
}

IRQn_Type Usart::GetIRQn() const
{
    if(mpUsart == USART1)
    {
        return USART1_IRQn;
    }
    else if(mpUsart == USART2)
    {
        return USART2_IRQn;
    }
    else if(mpUsart == USART3)
    {
        return USART3_IRQn;
    }
    else if(mpUsart == UART4)
    {
        return UART4_IRQn;
    }
    else if(mpUsart == UART5)
    {
        return UART5_IRQn;
    }
    else
    {
        ASSERT(0);
        return NonMaskableInt_IRQn;  // TODO: Fix me. return appopriate code or handle in some other way
    }
}

} // namespace USART
