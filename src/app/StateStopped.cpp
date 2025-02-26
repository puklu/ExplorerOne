#include "Bot.hpp"
#include "common/Trace.hpp"
#include "StateStopped.hpp"

StoppedState::StoppedState():StateBase(eStateName::STOPPED)
{
}

void StoppedState::DoWork(Bot *bot)
{
    bot->Stop();
    TRACE_LOG("Stopped");
}
