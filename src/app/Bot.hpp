#pragma once

#include <memory>

class IDrive;
// class IDistanceSensor;
class FSM;
class State;
class Transition;

class Bot
{
public:
    // Get the singleton instance
    static Bot* GetInstance();
    ~Bot();

    // delete copy and assignment operators
    Bot(const Bot&) = delete;
    Bot& operator=(const Bot&) = delete;


private:
    Bot();

    std::unique_ptr<IDrive> mpDrive;
    // std::unique_ptr<IDistanceSensor> mDistanceSensor;
    std::unique_ptr<FSM> mpFSM;
    std::shared_ptr<State> mpCurrentState;

    using GuardFunction = bool(*)(const Bot*);
    std::shared_ptr<Transition> CreateTransition(std::shared_ptr<State> from, std::shared_ptr<State> to, GuardFunction guard);

    static bool IsDistanceMoreThanThreshold(const Bot*);
    static bool IsDistanceLessThanThreshold(const Bot*);
    static bool IsEvaluationTime(const Bot*);
    static bool IsIdleTime(const Bot*);
};
