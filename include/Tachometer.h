#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <Arduino.h>

// Define how many pulses the fan generates per revolution.
// Most PC fans are 2 pulses per revolution.
#define PULSES_PER_REVOLUTION 2

class Tachometer
{
public:
    Tachometer(int tachPin);
    void begin();
    unsigned int getRPM();

    // Public ISR wrapper - must be called by a static ISR
    void pulseDetected_ISR_Wrapper();

private:
    int _tachPin;
    volatile unsigned long _pulseCount;
    volatile unsigned long _lastReadTime;

    // Static pointer to the instance for the ISR
    static Tachometer *_instance;

    // Static ISR function that calls the instance-specific wrapper
    static void actual_ISR();
};

#endif // TACHOMETER_H