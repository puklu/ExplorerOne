#pragma once

#include <string>

enum class eStateName
{
    CHECKING_FOR_OBSTACLE,
    MOVING_FORWARD,
    STOPPED,
    TURNING_TO_RIGHT
};

std::string StateEnumToString(eStateName stateName);

class State
{
public:
    State(eStateName stateName): mStateName(stateName){};
    virtual ~State();
    virtual void OnEntry();
    virtual void OnExit();
    virtual void DoWork() = 0;        

protected:
    eStateName mStateName;
};


class CheckingForObstacleState : public State
{
public:    
    CheckingForObstacleState();
    void DoWork() override;
};

class MovingForwardState : public State
{
public:     
    MovingForwardState();
    void DoWork() override;
};

class StoppedState : public State
{
public: 
    StoppedState();
    void DoWork() override;
};

class TurningToRightState : public State
{
public: 
    TurningToRightState();
    void DoWork() override;
};
