#include "common/defines.hpp"
#include "drivers/io/ExtiPin.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/io/ServiceISRs.hpp"


void ServiceISR0_4(uint8_t pinNum){
    uint8_t portIdx = 0;
    ExtiPin *pin = nullptr;

    for(portIdx = 0; portIdx < IO_PORT_COUNT; portIdx++)
    {
        // TODO: Assert that pin type in ExtiPin type  
        pin = static_cast<ExtiPin*>(activePins[portIdx][pinNum]);
        if(pin != nullptr && pin->isInterruptPresent()) 
        {
            InterruptCallback callback = pin->GetInterruptCallback();
            if(callback != nullptr){
               callback();
            }
            pin->ClearInterrupt();
        }
    }
}


void ServiceISR5_15(uint8_t startPinNumber, uint8_t endPinNumber){
    uint8_t portIdx = 0;
    ExtiPin *pin = nullptr;

    for(portIdx = 0; portIdx < IO_PORT_COUNT; portIdx++){
        for(uint8_t pinIdx = startPinNumber; pinIdx <= endPinNumber; pinIdx++)
        {
            // TODO: Assert that pin type in ExtiPin type    
            pin = static_cast<ExtiPin*>(activePins[portIdx][pinIdx]);
            if(pin != nullptr && pin->isInterruptPresent())
            {
                InterruptCallback callback = pin->GetInterruptCallback();
                if(callback != nullptr){
                callback();
            }
            pin->ClearInterrupt();
        }
     }
    }
}