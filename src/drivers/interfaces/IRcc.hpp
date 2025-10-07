#pragma once

#include "common/PinDefinitions.hpp"

class IRcc
{
   public:
    virtual ~IRcc()                                                  = default;
    virtual eGeneralStatus SelectSystemClock(eRccClockSource source) = 0;
    virtual eGeneralStatus SwitchOnClocks(eRccClocks clocks)         = 0;
    virtual eGeneralStatus SwitchOffClocks(eRccClocks clocks)        = 0;
    virtual eGeneralStatus SelectMcoClock(eRccClocks clock)          = 0;
    virtual eGeneralStatus TurnRtcOnOff()                            = 0;
    virtual eGeneralStatus SelectRtcClock(eRccClocks clock)          = 0;
    virtual uint32_t       GetSysClockFreq()                         = 0;
    virtual eGeneralStatus SetAhbPrescaler(eAhbPrescaler psc)        = 0;
    virtual uint32_t       GetAhbFrequency()                         = 0;
    virtual eGeneralStatus SetApb1Prescaler(eApb1Apb2Prescaler psc)  = 0;
    virtual uint32_t       GetApb1Frequency()                        = 0;
    virtual eGeneralStatus SetApb2Prescaler(eApb1Apb2Prescaler psc)  = 0;
    virtual uint32_t       GetApb2Frequency()                        = 0;
};
