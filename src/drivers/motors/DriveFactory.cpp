#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/motors/DriveFactory.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"

#include "drivers/leds/leds.hpp"


std::unique_ptr<Drive> DriveFactory::CreateMotorDrivers()
{
    ////////////// frontRight 
    // pin1
    GpioPinInitStruct frontMotorRightPwmPinInit = {};
    frontMotorRightPwmPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_A;
    frontMotorRightPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    frontMotorRightPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch1_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorRightPwmPinInit);

    auto pFrontMotorRightPwmPin = static_cast<GpioPin *>(tim2_ch1_pin);

    // pin2
    GpioPinInitStruct frontMotorRightDigitalPinInit = {};
    frontMotorRightDigitalPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_B;
    frontMotorRightDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    frontMotorRightDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *front_motor_right_digital_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorRightDigitalPinInit);

    auto pFrontMotorRightDigitalPin = static_cast<GpioPin *>(front_motor_right_digital_pin);

    ////////////// frontLeft
    // pin1
    GpioPinInitStruct frontMotorLeftPwmPinInit = {};
    frontMotorLeftPwmPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_LEFT_A;
    frontMotorLeftPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    frontMotorLeftPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch2_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorLeftPwmPinInit);

    auto pFrontMotorLeftPwmPin = static_cast<GpioPin *>(tim2_ch2_pin);

    // pin2
    GpioPinInitStruct frontMotorLeftDigitalPinInit = {};
    frontMotorLeftDigitalPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_LEFT_B;
    frontMotorLeftDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    frontMotorLeftDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *front_motor_left_digital_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorLeftDigitalPinInit);

    auto pFrontMotorLeftDigitalPin = static_cast<GpioPin *>(front_motor_left_digital_pin);

    ////////////// backRight 
    // pin1
    GpioPinInitStruct backMotorRightPwmPinInit = {};
    backMotorRightPwmPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_RIGHT_A;
    backMotorRightPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    backMotorRightPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch3_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorRightPwmPinInit);

    auto pBackMotorRightPwmPin = static_cast<GpioPin *>(tim2_ch3_pin);

    // pin2
    GpioPinInitStruct backMotorRightDigitalPinInit = {};
    backMotorRightDigitalPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_RIGHT_B;
    backMotorRightDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    backMotorRightDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *back_motor_right_digital_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorRightDigitalPinInit);

    auto pBackMotorRightDigitalPin = static_cast<GpioPin *>(back_motor_right_digital_pin);

    ////////////// backLeft
    // pin1
    GpioPinInitStruct backMotorLeftPwmPinInit = {};
    backMotorLeftPwmPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_LEFT_A;
    backMotorLeftPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    backMotorLeftPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch4_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorLeftPwmPinInit);

    auto pBackMotorLeftPwmPin = static_cast<GpioPin *>(tim2_ch4_pin);

    // pin2
    GpioPinInitStruct backMotorLeftDigitalPinInit = {};
    backMotorLeftDigitalPinInit.pin_name          = IO::ePin::IO_BACK_MOTOR_LEFT_B;
    backMotorLeftDigitalPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    backMotorLeftDigitalPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *back_motor_left_digital_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, backMotorLeftDigitalPinInit);

    auto pBackMotorLeftDigitalPin = static_cast<GpioPin *>(back_motor_left_digital_pin);


    GeneralPurposeTimerConfig pwm_timer_config;
    uint8_t frontMotorRightPwmChannelIndex = 0;
    uint8_t frontMotorLeftPwmChannelIndex = 1;
    uint8_t backMotorRightPwmChannelIndex = 2;
    uint8_t backMotorLeftPwmChannelIndex = 3;

    GpioPin *pwm_pins[GENERAL_PURPOSE_TIMER_NUM_CHANNELS] = {pFrontMotorRightPwmPin, pFrontMotorLeftPwmPin, pBackMotorRightPwmPin, pBackMotorLeftPwmPin};

    for(uint8_t i=0; i<4; i++)
    {
        pwm_timer_config.mChannels[i].mpChannelPin = pwm_pins[i];
        pwm_timer_config.mChannels[i].mAlternateFunction =
            IO::eAlternateFunction::IO_AF1;
        pwm_timer_config.mChannels[i].mSelection =
            Timer::eCaptureCompareSelection::OUTPUT;
        pwm_timer_config.mChannels[i]
            .mOutputCompareConfig.mOutputCompareMode =
            Timer::eOutputCompareMode::PWM_MODE_1;
        pwm_timer_config.mChannels[i].mCaptureCompareEnable =
            Timer::eCaptureCompare::ENABLE;
        pwm_timer_config.mChannels[i]
            .mOutputCompareConfig.mPwmDutyCyclePercent = 0;
        pwm_timer_config.mChannels[i].mOutputCompareConfig.mPwmPeriodMs =
            1;
        pwm_timer_config.mChannels[i]
            .mOutputCompareConfig.mOutputComparePreloadEnable =
            Timer::eOutputComparePreloadEnable::ENABLE;    
    }

    auto pPwmTimer = std::make_shared<GeneralPurposeTimer>(pwm_timer_config);

    ASSERT(pPwmTimer);

    pPwmTimer->Start();

    auto pFrontRightMotor = std::make_unique<Motor>(pPwmTimer, frontMotorRightPwmChannelIndex, pFrontMotorRightDigitalPin);
    auto pFrontLeftMotor = std::make_unique<Motor>(pPwmTimer, frontMotorLeftPwmChannelIndex, pFrontMotorLeftDigitalPin);
    auto pBackRightMotor = std::make_unique<Motor>(pPwmTimer, backMotorRightPwmChannelIndex, pBackMotorRightDigitalPin);
    auto pBackLeftMotor = std::make_unique<Motor>(pPwmTimer, backMotorLeftPwmChannelIndex, pBackMotorLeftDigitalPin);

    ASSERT(pFrontRightMotor);
    ASSERT(pFrontLeftMotor);
    ASSERT(pBackRightMotor);
    ASSERT(pBackLeftMotor);

    auto pDrive = std::make_unique<Drive>(std::move(pFrontRightMotor), std::move(pFrontLeftMotor), std::move(pBackRightMotor), std::move(pBackLeftMotor));
    
    ASSERT(pDrive);

    return pDrive;
}
