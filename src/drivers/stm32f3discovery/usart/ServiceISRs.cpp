#include <unordered_map>
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
    std::shared_ptr<UsartPin> pin = nullptr;

	for(auto &port : activeUsartPins)
    {
		for(auto &pinPtr : port)
		{
			// continue if the pin is not active
			if(pinPtr == nullptr)
			{
				continue;
			}

			pin = std::dynamic_pointer_cast<UsartPin>(pinPtr);

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
