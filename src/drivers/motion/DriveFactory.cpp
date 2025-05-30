// TODO: Move this file to platform specific folder?

#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"


std::unique_ptr<Mdd3aDrive> DriveFactory::CreateMdd3aDrive()
{
    ////////////// frontRight 
    // pin1: Configure the PWM pin for the front-right motor
    GpioPinInitStruct frontMotorRightPwmPinInit = {};
    frontMotorRightPwmPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_A;
    frontMotorRightPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    frontMotorRightPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> pFrontMotorRightPwmPin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorRightPwmPinInit);

    // pin2: Configure the digital pin for the front-right motor
    GpioPinInitStruct frontMotorRightDigitalPinInit = {};
    frontMotorRightDigitalPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_B;
    frontMotorRightDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    frontMotorRightDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    auto pFrontMotorRightDigitalPin = std::dynamic_pointer_cast<IDigitalOutputPin>(
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorRightDigitalPinInit));

    ////////////// frontLeft
    // pin1: Configure the PWM pin for the front-left motor
    GpioPinInitStruct frontMotorLeftPwmPinInit = {};
    frontMotorLeftPwmPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_LEFT_A;
    frontMotorLeftPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    frontMotorLeftPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> pFrontMotorLeftPwmPin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorLeftPwmPinInit);

    // pin2: Configure the digital pin for the front-left motor
    GpioPinInitStruct frontMotorLeftDigitalPinInit = {};
    frontMotorLeftDigitalPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_LEFT_B;
    frontMotorLeftDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    frontMotorLeftDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    auto pFrontMotorLeftDigitalPin = std::dynamic_pointer_cast<IDigitalOutputPin>(
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorLeftDigitalPinInit));

    ////////////// backRight 
    // pin1: Configure the PWM pin for the back-right motor
    GpioPinInitStruct backMotorRightPwmPinInit = {};
    backMotorRightPwmPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_RIGHT_A;
    backMotorRightPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    backMotorRightPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> pBackMotorRightPwmPin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorRightPwmPinInit);

    // pin2: Configure the digital pin for the back-right motor
    GpioPinInitStruct backMotorRightDigitalPinInit = {};
    backMotorRightDigitalPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_RIGHT_B;
    backMotorRightDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    backMotorRightDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    auto pBackMotorRightDigitalPin = std::dynamic_pointer_cast<IDigitalOutputPin>(
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorRightDigitalPinInit));

    ////////////// backLeft
    // pin1: Configure the PWM pin for the back-left motor
    GpioPinInitStruct backMotorLeftPwmPinInit = {};
    backMotorLeftPwmPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_LEFT_A;
    backMotorLeftPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    backMotorLeftPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> pBackMotorLeftPwmPin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorLeftPwmPinInit);

    // pin2: Configure the digital pin for the back-left motor
    GpioPinInitStruct backMotorLeftDigitalPinInit = {};
    backMotorLeftDigitalPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_LEFT_B;
    backMotorLeftDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    backMotorLeftDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    auto pBackMotorLeftDigitalPin = std::dynamic_pointer_cast<IDigitalOutputPin>(
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorLeftDigitalPinInit));

    // Instance to hold all the pwm timer configurations which will be used to configure the PWM instace
    GeneralPurposeTimerConfig pwm_timer_config;

    // Use all the four channels of the same timer since we need 4 PWM pins
    uint8_t frontMotorRightPwmChannelIndex = 0;
    uint8_t frontMotorLeftPwmChannelIndex = 1;
    uint8_t backMotorRightPwmChannelIndex = 2;
    uint8_t backMotorLeftPwmChannelIndex = 3;

    std::array<std::shared_ptr<PinBase>, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> pwm_pins= {pFrontMotorRightPwmPin, pFrontMotorLeftPwmPin, pBackMotorRightPwmPin, pBackMotorLeftPwmPin};

    // Fill up the timer config instance for all the channels
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {
        pwm_timer_config.mChannels[i]->mpChannelPin = pwm_pins[i];
        pwm_timer_config.mChannels[i]->mAlternateFunction =
            IO::eAlternateFunction::IO_AF1;
        pwm_timer_config.mChannels[i]->mSelection =
            Timer::eCaptureCompareSelection::OUTPUT;
        pwm_timer_config.mChannels[i]
            ->mOutputCompareConfig.mOutputCompareMode =
            Timer::eOutputCompareMode::PWM_MODE_1;
        pwm_timer_config.mChannels[i]->mCaptureCompareEnable =
            Timer::eCaptureCompare::ENABLE;
        pwm_timer_config.mChannels[i]
            ->mOutputCompareConfig.mPwmDutyCyclePercent = 0;
        pwm_timer_config.mChannels[i]->mOutputCompareConfig.mPwmPeriodMs = 1.0f;
        pwm_timer_config.mChannels[i]
            ->mOutputCompareConfig.mOutputComparePreloadEnable =
            Timer::eOutputComparePreloadEnable::ENABLE;    
    }

    // Create the PWM timer using the timer config from above
    auto pPwmTimer = std::make_shared<GeneralPurposeTimer>(pwm_timer_config);

    ASSERT(pPwmTimer);

    // Start the PWM timer. At this point the channels start outputting PWM
    pPwmTimer->Start();

    // Create four motor instances for the four motors
    // Same PWM timer instace is passed to all the four motors since there
    // are enough (four) PWM channels present in the same timer.
    // Just a different channel is passed to each motor
    auto pFrontRightMotor = std::make_unique<Motor>(pPwmTimer, frontMotorRightPwmChannelIndex, pFrontMotorRightDigitalPin);
    auto pFrontLeftMotor = std::make_unique<Motor>(pPwmTimer, frontMotorLeftPwmChannelIndex, pFrontMotorLeftDigitalPin);
    auto pBackRightMotor = std::make_unique<Motor>(pPwmTimer, backMotorRightPwmChannelIndex, pBackMotorRightDigitalPin);
    auto pBackLeftMotor = std::make_unique<Motor>(pPwmTimer, backMotorLeftPwmChannelIndex, pBackMotorLeftDigitalPin);

    ASSERT(pFrontRightMotor);
    ASSERT(pFrontLeftMotor);
    ASSERT(pBackRightMotor);
    ASSERT(pBackLeftMotor);

    // Create the Mdd3aDrive instance using the four motors from above
    auto pDrive = std::make_unique<Mdd3aDrive>(std::move(pFrontRightMotor), std::move(pFrontLeftMotor), std::move(pBackRightMotor), std::move(pBackLeftMotor));
    
    ASSERT(pDrive);

    return pDrive;
}
