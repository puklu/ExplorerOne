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

class State
{
public:
    State(eStateName stateName): mStateName(stateName){};
    virtual ~State();
    virtual void OnEntry();
    virtual void OnExit();
    virtual void DoWork(Bot*) = 0;        

protected:
    eStateName mStateName;
};


class CheckingForObstacleState : public State
{
public:    
    CheckingForObstacleState();
    void DoWork(Bot *bot) override;   
};

class MovingForwardState : public State
{
public:     
    MovingForwardState();
    void DoWork(Bot *bot) override;
};

class StoppedState : public State
{
public: 
    StoppedState();
    void DoWork(Bot *bot) override;
};

class TurningToRightState : public State
{
public: 
    TurningToRightState();
    void DoWork(Bot *bot) override;
};

class TurningToLeftState : public State
{
public: 
    TurningToLeftState();
    void DoWork(Bot *bot) override;
};
