#include "common/assertHandler.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/motors/DriveFactory.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"


Drive * DriveFactory::CreateMotorDrivers()
{
    ////////////// frontRight 
    // pin1
    GpioPinInitStruct frontMotorRightPwmPinInit = {};
    frontMotorRightPwmPinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_A;
    frontMotorRightPwmPinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    frontMotorRightPwmPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch1_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, frontMotorRightPwmPinInit);

    auto frontMotorRightPwmPin = static_cast<GpioPin *>(tim2_ch1_pin);

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

    auto frontMotorLeftPwmPin = static_cast<GpioPin *>(tim2_ch2_pin);

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

    auto backMotorRightPwmPin = static_cast<GpioPin *>(tim2_ch3_pin);

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

    auto backMotorLeftPwmPin = static_cast<GpioPin *>(tim2_ch4_pin);

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

    GpioPin *pwm_pins[4] = {frontMotorRightPwmPin, frontMotorLeftPwmPin, backMotorRightPwmPin, backMotorLeftPwmPin};

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
            2;
        pwm_timer_config.mChannels[i]
            .mOutputCompareConfig.mOutputComparePreloadEnable =
            Timer::eOutputComparePreloadEnable::ENABLE;    
    }

    // pwm_timer_config.mTriggerInterrupt = Timer::eTriggerInterrupt::DISABLE;
    // pwm_timer_config.mEnableUpdateInterrupt = Timer::eUpdateInterrupt::DISABLE;

    GeneralPurposeTimer pwm_timer(pwm_timer_config);

    pwm_timer.Start();

    Motor *frontRight = new Motor(&pwm_timer, frontMotorRightPwmChannelIndex, pFrontMotorRightDigitalPin);
    Motor *frontLeft = new Motor(&pwm_timer, frontMotorLeftPwmChannelIndex, pFrontMotorLeftDigitalPin);
    Motor *backRight = new Motor(&pwm_timer, backMotorRightPwmChannelIndex, pBackMotorRightDigitalPin);
    Motor *backLeft = new Motor(&pwm_timer, backMotorLeftPwmChannelIndex, pBackMotorLeftDigitalPin);

    return new Drive(frontRight, frontLeft, backRight, backLeft);
}
