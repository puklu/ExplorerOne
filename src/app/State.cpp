#include "Bot.hpp"
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
    
    case eStateName::TURNING_TO_LEFT:
        return "TURNING_TO_LEFT"; 
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

void CheckingForObstacleState::DoWork(Bot *bot)
{
    (void)bot;
    TRACE_LOG("Checking For Obstacle");
}


MovingForwardState::MovingForwardState():State(eStateName::MOVING_FORWARD)
{
}

void MovingForwardState::DoWork(Bot *bot)
{
    TRACE_LOG("Moving forward");
    bot->MoveForward(MOVING_FORWARD_SPEED);
}



StoppedState::StoppedState():State(eStateName::STOPPED)
{
}

void StoppedState::DoWork(Bot *bot)
{
    bot->Stop();
    TRACE_LOG("Stopped");
}



TurningToRightState::TurningToRightState():State(eStateName::TURNING_TO_RIGHT)
{
}

void TurningToRightState::DoWork(Bot *bot)
{
    TRACE_LOG("Turning to right");
    bot->TurnRight(TURNING_SPEED, TURNING_RADIUS);
    bot->SetLastTurnDirection(Bot::eLastTurn::RIGHT);
}

TurningToLeftState::TurningToLeftState():State(eStateName::TURNING_TO_LEFT)
{
}

void TurningToLeftState::DoWork(Bot *bot)
{   
    TRACE_LOG("Turning to left");
    bot->TurnLeft(TURNING_SPEED, TURNING_RADIUS);
    bot->SetLastTurnDirection(Bot::eLastTurn::LEFT);
}
