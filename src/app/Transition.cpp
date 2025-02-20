#include "Transition.hpp"

Transition::Transition(std::shared_ptr<State> from, std::shared_ptr<State> to, GuardFunction guard):
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

std::shared_ptr<State> Transition::GetFromState()
{
    return mpFrom;
}

std::shared_ptr<State> Transition::GetToState()
{
    return mpTo;
}
