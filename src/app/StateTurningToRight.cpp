#include "Bot.hpp"
#include "common/Trace.hpp"
#include "StateTurningToRight.hpp"

TurningToRightState::TurningToRightState():StateBase(eStateName::TURNING_TO_RIGHT)
{
}

void TurningToRightState::DoWork(Bot *bot)
{
    TRACE_LOG("Turning to right");
    bot->TurnRight(TURNING_SPEED, TURNING_RADIUS);
    bot->SetLastTurnDirection(Bot::eLastTurn::RIGHT);
}
