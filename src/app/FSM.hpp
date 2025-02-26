#pragma once

#include <memory>
#include <vector>

class Bot;
class StateBase;
class Transition;

class FSM
{
public:
    FSM();
    ~FSM();
    void Initialize(std::shared_ptr<StateBase> initialState);
    void AddState(std::shared_ptr<StateBase> state);
    void AddTransition(std::shared_ptr<Transition> transition);
    void HandleEvent(Bot &bot);
    std::shared_ptr<StateBase> GetCurrentState() const;

private:
    std::shared_ptr<StateBase> mpCurrentState;
    std::vector<std::shared_ptr<StateBase>> mpStates;
    std::vector<std::shared_ptr<Transition>> mpTransitions;
};
