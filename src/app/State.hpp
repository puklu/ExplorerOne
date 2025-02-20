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
    virtual ~State() = default;
    virtual void OnEntry();
    virtual void OnExit();
    virtual void DoWork() = 0;        

protected:
    eStateName mStateName;
};


class CheckingForObstacleState : public State
{
    CheckingForObstacleState();
    void DoWork() override;
};

class MovingForwardState : public State
{
    MovingForwardState();
    void DoWork() override;
};

class StoppedState : public State
{
    StoppedState();
    void DoWork() override;
};

class TurningToRightState : public State
{
    TurningToRightState();
    void DoWork() override;
};
