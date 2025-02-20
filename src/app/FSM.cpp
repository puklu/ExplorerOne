#include "FSM.hpp"
#include "State.hpp"
#include "Transition.hpp"

FSM::FSM()
{

}

FSM::~FSM() = default;

void FSM::Initialize(std::shared_ptr<State> initialState)
{
    mpCurrentState = std::move(initialState);
    mpStates.push_back(std::move(initialState));
}

void FSM::AddState(std::shared_ptr<State> state)
{
    mpStates.push_back(std::move(state));
}

void FSM::AddTransition(std::shared_ptr<Transition> transition)
{
    mpTransitions.push_back(std::move(transition));
}

void FSM::HandleEvent(Bot &bot)
{
    for(auto transition :mpTransitions)
    {
        if(transition->GetFromState() == mpCurrentState and transition->CanTransition(&bot))
        {
            mpCurrentState->OnExit();
            mpCurrentState = transition->GetToState();
            mpCurrentState->OnEntry();
            mpCurrentState->DoWork();
            break;
        }
    }
}

std::shared_ptr<State> FSM::GetCurrentState() const
{
    return mpCurrentState;
}