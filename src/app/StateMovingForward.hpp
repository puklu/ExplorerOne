#pragma once

#include "StateBase.hpp"

class MovingForwardState : public StateBase
{
public:     
    MovingForwardState();
    void DoWork(Bot *bot) override;
};
