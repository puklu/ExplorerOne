#pragma once

#include <memory>
#include "common/PinDefinitions.hpp"

class IDrive;
class IDistanceSensor;
class FSM;
class StateBase;
class Transition;

#define THRESHOLD_DISTANCE_MM 20
#define MOVING_FORWARD_SPEED 80
#define TURNING_SPEED 80
#define TURNING_RADIUS eTurnRadius::ZERO

class Bot
{
public:
    // Get the singleton instance
    static Bot* GetInstance();
    ~Bot();

    void Run();

    enum class eLastTurn
    {
        RIGHT,
        LEFT
    };

    void SetLastTurnDirection(eLastTurn turnDirection);
    void MoveForward(int8_t speed_percent);
    void Stop();
    void TurnRight(int8_t speed_percent, eTurnRadius turn_radius);
    void TurnLeft(int8_t speed_percent, eTurnRadius turn_radius);

    // delete copy and assignment operators
    Bot(const Bot&) = delete;
    Bot& operator=(const Bot&) = delete;


private:
    Bot();

    std::unique_ptr<IDrive> mpDrive;
    std::unique_ptr<IDistanceSensor> mpDistanceSensor;
    std::unique_ptr<FSM> mpFSM;
    std::shared_ptr<StateBase> mpCurrentState;
    float mDistanceToObstacle;

    eLastTurn mLastTurnDirection = eLastTurn::RIGHT;

    using EventFunction = bool(*)(const Bot*);
    std::shared_ptr<Transition> CreateTransition(std::shared_ptr<StateBase> from, std::shared_ptr<StateBase> to, EventFunction event);

    static bool IsDistanceMoreThanThreshold(const Bot*);
    static bool IsDistanceLessThanThreshold(const Bot*);
    static bool IsEvaluationTime(const Bot*);
    static bool IsIdleTime(const Bot*);
    static bool ShouldTurnToRight(const Bot* bot);
    static bool ShouldTurnToLeft(const Bot* bot);
};

