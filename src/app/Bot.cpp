#include "Bot.hpp"
#include "FSM.hpp"
#include "State.hpp"
#include "Transition.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"

#include "drivers/interfaces/IDrive.hpp"
#include "drivers/motion/DriveFactory.hpp"
// #include "drivers/sensors/Ultrasonic.hpp"
#include "UltrasonicSensorStub.hpp"  // TODO: Replace stub by actual implementation


Bot* Bot::GetInstance()
{
   static Bot instance; // Thread-safe local static instance
   return &instance;

}

Bot::Bot():
    mpDrive(DriveFactory::CreateMdd3aDrive()),
    mpDistanceSensor(SensorFactory::CreateUltrasonicSensor()),
    mpFSM(std::make_unique<FSM>())
{

    // create pointers to states the Bot can be in
    // std::shared_ptr<State> pCheckingForObstacleState = std::make_shared<CheckingForObstacleState>();
    std::shared_ptr<State> pMovingForwardState = std::make_shared<MovingForwardState>();
    std::shared_ptr<State> pStoppedState = std::make_shared<StoppedState>();
    std::shared_ptr<State> pTurningToRightState = std::make_shared<TurningToRightState>();
    std::shared_ptr<State> pTurningToLeftState = std::make_shared<TurningToLeftState>();

    // create all the possible transitions
    // std::shared_ptr<Transition> pCheckingForObstacleToMovingForward = CreateTransition(pCheckingForObstacleState, pMovingForwardState, IsDistanceMoreThanThreshold);
    // std::shared_ptr<Transition> pCheckingForObstacleToStoppedForward = CreateTransition(pCheckingForObstacleState, pStoppedState, IsDistanceLessThanThreshold);
    // std::shared_ptr<Transition> pMovingForwardToCheckingForObstacle = CreateTransition(pMovingForwardState, pCheckingForObstacleState, IsEvaluationTime);
    // std::shared_ptr<Transition> pTurningToRightToCheckingForObstacle = CreateTransition(pTurningToRightState, pCheckingForObstacleState, IsEvaluationTime);
    std::shared_ptr<Transition> pMovingForwardToStopped = CreateTransition(pMovingForwardState, pStoppedState, IsDistanceLessThanThreshold);
    std::shared_ptr<Transition> pTurningToRightToMovingForward = CreateTransition(pTurningToRightState, pMovingForwardState, IsDistanceMoreThanThreshold);
    std::shared_ptr<Transition> pTurningToLeftToMovingForward = CreateTransition(pTurningToLeftState, pMovingForwardState, IsDistanceMoreThanThreshold);
    std::shared_ptr<Transition> pStoppedStateToTurningToRight = CreateTransition(pStoppedState, pTurningToRightState, ShouldTurnToRight);
    std::shared_ptr<Transition> pStoppedStateToTurningToLeft = CreateTransition(pStoppedState, pTurningToLeftState, ShouldTurnToLeft);

    // add the states and transition to the FSM
    // mpFSM->AddState(pCheckingForObstacleState);
    mpFSM->AddState(pStoppedState);
    mpFSM->AddState(pMovingForwardState);
    mpFSM->AddState(pTurningToRightState);
    mpFSM->AddState(pTurningToLeftState);
    // mpFSM->AddTransition(pCheckingForObstacleToMovingForward);
    // mpFSM->AddTransition(pCheckingForObstacleToStoppedForward);
    // mpFSM->AddTransition(pMovingForwardToCheckingForObstacle);
    // mpFSM->AddTransition(pTurningToRightToCheckingForObstacle);
    mpFSM->AddTransition(pMovingForwardToStopped);
    mpFSM->AddTransition(pTurningToRightToMovingForward);
    mpFSM->AddTransition(pStoppedStateToTurningToRight);
    mpFSM->AddTransition(pStoppedStateToTurningToLeft);
    mpFSM->AddTransition(pTurningToLeftToMovingForward);


    // mpCurrentState = pCheckingForObstacleState;
    mpCurrentState = pMovingForwardState;
    mDistanceToObstacle = 0;

    mpFSM->Initialize(mpCurrentState);

}

std::shared_ptr<Transition> Bot::CreateTransition(std::shared_ptr<State> from, std::shared_ptr<State> to, EventFunction event)
{
    return std::make_shared<Transition>(from, to, event);
}


Bot::~Bot() = default;

bool Bot::IsDistanceMoreThanThreshold(const Bot* bot)
{
    float distance = bot->mpDistanceSensor->GetDistanceInMm();
    TRACE_LOG("Obstacle distance is: %.2f", distance);
    return (distance > THRESHOLD_DISTANCE_MM);
}

bool Bot::IsDistanceLessThanThreshold(const Bot* bot)
{
    float distance = bot->mpDistanceSensor->GetDistanceInMm();
    TRACE_LOG("Obstacle distance is: %.2f", distance);
    return (distance < THRESHOLD_DISTANCE_MM);
}

bool Bot::IsEvaluationTime(const Bot* bot)
{
    (void)bot;
    return true;
}

bool Bot::IsIdleTime(const Bot* bot)
{
    (void)bot;
    delay(1000);
    return true;
}

bool Bot::ShouldTurnToRight(const Bot* bot)
{
    delay(1000);
    bool resultToReturn = (bot->mLastTurnDirection == eLastTurn::LEFT) ? true : false;
    return resultToReturn;
}

bool Bot::ShouldTurnToLeft(const Bot* bot)
{
    delay(1000);
    bool resultToReturn = (bot->mLastTurnDirection == eLastTurn::RIGHT) ? true : false;
    return resultToReturn;
}

void Bot::Run()
{
    mpFSM->HandleEvent(*this);
}

void Bot::SetLastTurnDirection(eLastTurn turnDirection)
{
    mLastTurnDirection = turnDirection;
}

void Bot::MoveForward(int8_t speed_percent)
{
    mpDrive->Forward(speed_percent);
}

void Bot::Stop()
{
    mpDrive->Halt();
}

void Bot::TurnRight(int8_t speed_percent, eTurnRadius turn_radius)
{
    mpDrive->Right(speed_percent, turn_radius);
}

void Bot::TurnLeft(int8_t speed_percent, eTurnRadius turn_radius)
{
    mpDrive->Left(speed_percent, turn_radius);
}
