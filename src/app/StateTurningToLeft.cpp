#include "Bot.hpp"
#include "common/Trace.hpp"
#include "StateTurningToLeft.hpp"

TurningToLeftState::TurningToLeftState():StateBase(eStateName::TURNING_TO_LEFT)
{
}

void TurningToLeftState::DoWork(Bot *bot)
{   
    TRACE_LOG("Turning to left");
    bot->TurnLeft(TURNING_SPEED, TURNING_RADIUS);
    bot->SetLastTurnDirection(Bot::eLastTurn::LEFT);
}
