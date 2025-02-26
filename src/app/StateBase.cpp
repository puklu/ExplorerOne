#include "Bot.hpp"
#include "StateBase.hpp"
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


StateBase::~StateBase() = default;

void StateBase::OnEntry()
{
    TRACE_LOG("Entered state: %s", StateEnumToString(mStateName).c_str());
}

void StateBase::OnExit()
{
    TRACE_LOG("Exited state: %s", StateEnumToString(mStateName).c_str());
}
