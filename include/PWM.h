#ifndef PWM_FAN_CONTROL_H
#define PWM_FAN_CONTROL_H

#include <Arduino.h>

class PWMFanControl
{
public:
    PWMFanControl(int pwmPin);
    void begin();
    void setSpeed(int percentage);       // Speed from 0 to 100%
    void setRawDutyCycle(int dutyCycle); // Raw PWM value (0-255 for Arduino Nano)
    void off();

private:
    int _pwmPin;
};

#endif // PWM_FAN_CONTROL_H