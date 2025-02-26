#include "Bot.hpp"
#include "common/Trace.hpp"
#include "StateMovingForward.hpp"

MovingForwardState::MovingForwardState():StateBase(eStateName::MOVING_FORWARD)
{
}

void MovingForwardState::DoWork(Bot *bot)
{
    TRACE_LOG("Moving forward");
    bot->MoveForward(MOVING_FORWARD_SPEED);
}
