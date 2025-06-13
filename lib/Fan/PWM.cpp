// 3pin pc fan (SU1225FD12M-RH ; 12v 0.13a) controlled with IRLZ44N MOSFET
#include "PWM.h"

PWMFanControl::PWMFanControl(int pwmPin)
{
    _pwmPin = pwmPin;
}

void PWMFanControl::begin()
{
    pinMode(_pwmPin, OUTPUT);
    digitalWrite(_pwmPin, LOW); // Start with fan off
}

/**
 * @brief Sets the fan speed as a percentage (0-100%).
 * @param percentage The desired speed percentage.
 */
void PWMFanControl::setSpeed(int percentage)
{
    if (percentage < 0)
        percentage = 0;
    if (percentage > 100)
        percentage = 100;

    // Map percentage (0-100) to PWM duty cycle (0-255)
    int dutyCycle = map(percentage, 0, 100, 0, 255);
    setRawDutyCycle(dutyCycle);
}

/**
 * @brief Sets the raw PWM duty cycle (0-255 for Arduino Nano).
 * @param dutyCycle The PWM duty cycle value.
 */
void PWMFanControl::setRawDutyCycle(int dutyCycle)
{
    if (dutyCycle < 0)
        dutyCycle = 0;
    if (dutyCycle > 255)
        dutyCycle = 255; // Max for 8-bit PWM

    analogWrite(_pwmPin, dutyCycle);
}

/**
 * @brief Turns the fan off.
 */
void PWMFanControl::off()
{
    setRawDutyCycle(0);
}