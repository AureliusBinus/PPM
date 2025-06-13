#ifndef PWM_FAN_CONTROL_H
#define PWM_FAN_CONTROL_H

#include <Arduino.h>

class PWMFanControl
{
public:
    PWMFanControl(int pwmPin); // Added constructor
    void begin();
    void setSpeed(int percentage);
    void setRawDutyCycle(int dutyCycle);
    void off();

private:
    int _pwmPin;
};

#endif // PWM_FAN_CONTROL_H