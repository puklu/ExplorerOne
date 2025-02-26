#include "Transition.hpp"

Transition::Transition(std::shared_ptr<StateBase> from, std::shared_ptr<StateBase> to, GuardFunction guard):
    mpFrom(from),
    mpTo(to),
    mpGuard(guard)
{
}

Transition::~Transition() = default;

bool Transition::CanTransition(const Bot *bot) const
{
    return mpGuard(bot);
}

std::shared_ptr<StateBase> Transition::GetFromState()
{
    return mpFrom;
}

std::shared_ptr<StateBase> Transition::GetToState()
{
    return mpTo;
}
