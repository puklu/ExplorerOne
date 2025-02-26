#pragma once

#include "StateBase.hpp"

class StoppedState : public StateBase
{
public: 
    StoppedState();
    void DoWork(Bot *bot) override;
};
