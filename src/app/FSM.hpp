#pragma once

#include <memory>
#include <vector>

class Bot;
class State;
class Transition;

class FSM
{
public:
    FSM();
    ~FSM();
    void Initialize(std::shared_ptr<State> initialState);
    void AddState(std::shared_ptr<State> state);
    void AddTransition(std::shared_ptr<Transition> transition);
    void HandleEvent(Bot &bot);
    std::shared_ptr<State> GetCurrentState() const;

private:
    std::shared_ptr<State> mpCurrentState;
    std::vector<std::shared_ptr<State>> mpStates;
    std::vector<std::shared_ptr<Transition>> mpTransitions;
};
