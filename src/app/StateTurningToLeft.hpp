#pragma once

#include "StateBase.hpp"

class TurningToLeftState : public StateBase
{
public: 
    TurningToLeftState();
    void DoWork(Bot *bot) override;
};
