#pragma once

#include <memory>

class Bot;
class State;

class Transition
{
public:
    using GuardFunction = bool(*)(const Bot*);

    Transition(std::shared_ptr<State> from, std::shared_ptr<State> to, GuardFunction guard);
    ~Transition();
    bool CanTransition(const Bot *bot) const;
    std::shared_ptr<State> GetFromState();
    std::shared_ptr<State> GetToState();
    
private:
    std::shared_ptr<State> mpFrom;
    std::shared_ptr<State> mpTo;
    GuardFunction mpGuard;
};
