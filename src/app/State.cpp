#include "State.hpp"
#include "common/Trace.hpp"

std::string StateEnumToString(eStateName stateName)
{
    switch (stateName)
    {
    case eStateName::CHECKING_FOR_OBSTACLE:
        return "CHECKING_FOR_OBSTACLE";
    
    case eStateName::MOVING_FORWARD:
        return "MOVING_FORWARD";

    case eStateName::STOPPED:
        return "STOPPED";
    
    case eStateName::TURNING_TO_RIGHT:
        return "TURNING_TO_RIGHT";        
    default:
        return "UNKNOWN_STATE";
    }
}


State::~State() = default;

void State::OnEntry()
{
    TRACE_LOG("Entered state: %s", StateEnumToString(mStateName).c_str());
}

void State::OnExit()
{
    TRACE_LOG("Exited state: %s", StateEnumToString(mStateName).c_str());
}


CheckingForObstacleState::CheckingForObstacleState():State(eStateName::CHECKING_FOR_OBSTACLE)
{
}

void CheckingForObstacleState::DoWork()
{
    TRACE_LOG("Checking For Obstacle");
}


MovingForwardState::MovingForwardState():State(eStateName::MOVING_FORWARD)
{
}

void MovingForwardState::DoWork()
{
    TRACE_LOG("Moving forward");
}



StoppedState::StoppedState():State(eStateName::STOPPED)
{
}

void StoppedState::DoWork()
{
    TRACE_LOG("Stopped");
}



TurningToRightState::TurningToRightState():State(eStateName::TURNING_TO_RIGHT)
{
}

void TurningToRightState::DoWork()
{
    TRACE_LOG("Turning to right");
}
