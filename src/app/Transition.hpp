#pragma once

#include <memory>

class Bot;
class StateBase;

class Transition
{
public:
    using GuardFunction = bool(*)(const Bot*);

    Transition(std::shared_ptr<StateBase> from, std::shared_ptr<StateBase> to, GuardFunction guard);
    ~Transition();
    bool CanTransition(const Bot *bot) const;
    std::shared_ptr<StateBase> GetFromState();
    std::shared_ptr<StateBase> GetToState();
    
private:
    std::shared_ptr<StateBase> mpFrom;
    std::shared_ptr<StateBase> mpTo;
    GuardFunction mpGuard;
};
