#pragma once

#include "StateBase.hpp"

class TurningToRightState : public StateBase
{
public: 
    TurningToRightState();
    void DoWork(Bot *bot) override;
};
