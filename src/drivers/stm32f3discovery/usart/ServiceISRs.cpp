#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/usart/UsartPin.hpp"
#include "ServiceISRs.hpp"


void UsartServiceISR_Printing()
{
	ASSERT(activePrintUsartPin != nullptr);
    USART_TypeDef* usart = activePrintUsartPin->GetSelectedUsart();

	if((usart->ISR & (USART_ISR_TXE)))  // if the interrupt telling that transmit data register is active.
	{
		char nextData;
		if(activePrintUsartPin->GetRingBuffer()->take(nextData)== eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS)
		{
			usart->TDR = nextData;
		}
		else
		{
			activePrintUsartPin->DisableTxRegisterEmptyInterrupt(); // if there is no data to transmit, disable the interrupt which tells that transmit data register is empty.
		}
     }
}


void Usart1ServiceISR()
{
	ASSERT(0);
}


void Usart2ServiceISR()
{
	ASSERT(0);
}


void Usart3ServiceISR()
{
	ASSERT(0);
}


void Uart4ServiceISR()
{
	ASSERT(0);
}


void Uart5ServiceISR()
{
	ASSERT(0);
}


void UsartServiceISR()
{

	uint8_t portIdx;
	uint8_t pinIdx;
    std::shared_ptr<UsartPin> pin = nullptr;

    for(portIdx = 0; portIdx < IO_PORT_COUNT; portIdx++)
    {
		for(pinIdx = 0; pinIdx < IO_PIN_COUNT_PER_PORT; pinIdx++)
        {
			pin = std::dynamic_pointer_cast<UsartPin>(activeUsartPins[portIdx][pinIdx]);
			if(pin == nullptr)
			{
				continue;
			}
			if (pin == activePrintUsartPin)
			{
				UsartServiceISR_Printing();
				return;
			}

			InterruptCallback callback = pin->GetInterruptCallback();
			if(callback != nullptr){
				callback();
			}
		}
    }
}
