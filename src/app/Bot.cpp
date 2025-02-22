#include "Bot.hpp"
#include "FSM.hpp"
#include "State.hpp"
#include "Transition.hpp"

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
    std::shared_ptr<State> pCheckingForObstacleState = std::make_shared<CheckingForObstacleState>();
    std::shared_ptr<State> pMovingForwardState = std::make_shared<MovingForwardState>();
    std::shared_ptr<State> pStoppedState = std::make_shared<StoppedState>();
    std::shared_ptr<State> pTurningToRightState = std::make_shared<TurningToRightState>();

    // create all the possible transitions
    std::shared_ptr<Transition> pCheckingForObstacleToMovingForward = CreateTransition(pCheckingForObstacleState, pMovingForwardState, IsDistanceMoreThanThreshold);
    std::shared_ptr<Transition> pCheckingForObstacleToStoppedForward = CreateTransition(pCheckingForObstacleState, pStoppedState, IsDistanceLessThanThreshold);
    std::shared_ptr<Transition> pMovingForwardToCheckingForObstacle = CreateTransition(pMovingForwardState, pCheckingForObstacleState, IsEvaluationTime);
    std::shared_ptr<Transition> pStoppedStateToTurningToRight = CreateTransition(pStoppedState, pTurningToRightState, IsIdleTime);
    std::shared_ptr<Transition> pTurningToRightToCheckingForObstacle = CreateTransition(pTurningToRightState, pCheckingForObstacleState, IsEvaluationTime);

    // add the states and transition to the FSM
    mpFSM->AddState(pCheckingForObstacleState);
    mpFSM->AddState(pMovingForwardState);
    mpFSM->AddState(pStoppedState);
    mpFSM->AddState(pTurningToRightState);
    mpFSM->AddTransition(pCheckingForObstacleToMovingForward);
    mpFSM->AddTransition(pCheckingForObstacleToStoppedForward);
    mpFSM->AddTransition(pMovingForwardToCheckingForObstacle);
    mpFSM->AddTransition(pStoppedStateToTurningToRight);
    mpFSM->AddTransition(pTurningToRightToCheckingForObstacle);

    mpCurrentState = pCheckingForObstacleState;

    mpFSM->Initialize(mpCurrentState);

}

std::shared_ptr<Transition> Bot::CreateTransition(std::shared_ptr<State> from, std::shared_ptr<State> to, GuardFunction guard)
{
    return std::make_shared<Transition>(from, to, guard);
}


Bot::~Bot() = default;

bool Bot::IsDistanceMoreThanThreshold(const Bot* bot)
{
    return (bot->mpDistanceSensor->GetDistanceInMm() > THRESHOLD_DISTANCE_MM);
}

bool Bot::IsDistanceLessThanThreshold(const Bot* bot)
{
    return (bot->mpDistanceSensor->GetDistanceInMm() < THRESHOLD_DISTANCE_MM);
}

bool Bot::IsEvaluationTime(const Bot* bot)
{
    (void)bot;
    return true;
}

bool Bot::IsIdleTime(const Bot* bot)
{
    (void)bot;
    return true;
}
