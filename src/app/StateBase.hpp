#pragma once

#include <string>

class Bot;

enum class eStateName
{
    CHECKING_FOR_OBSTACLE,
    MOVING_FORWARD,
    STOPPED,
    TURNING_TO_RIGHT,
    TURNING_TO_LEFT
};

std::string StateEnumToString(eStateName stateName);

class StateBase
{
public:
    StateBase(eStateName stateName): mStateName(stateName){};
    virtual ~StateBase();
    virtual void OnEntry();
    virtual void OnExit();
    virtual void DoWork(Bot*) = 0;        

protected:
    eStateName mStateName;
};






