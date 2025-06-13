// 3pin pc fan (SU1225FD12M-RH ; 12v 0.13a)
#include "Tachometer.h"

// Initialize static member
Tachometer *Tachometer::_instance = nullptr;

Tachometer::Tachometer(int tachPin)
{
    _tachPin = tachPin;
    _pulseCount = 0;
    _lastReadTime = 0;
    _instance = this; // Store this instance for the static ISR
}

// Static ISR that calls the member function on the stored instance
void Tachometer::actual_ISR()
{
    if (_instance)
    {
        _instance->pulseDetected_ISR_Wrapper();
    }
}

// Member function called by the static ISR
void Tachometer::pulseDetected_ISR_Wrapper()
{
    _pulseCount++;
}

void Tachometer::begin()
{
    pinMode(_tachPin, INPUT_PULLUP); // Tachometer pins usually need a pull-up
    _lastReadTime = millis();
    // Attach interrupt to the tachometer pin.
    // Ensure the pin chosen supports interrupts on Arduino Nano (typically D2 or D3).
    // digitalPinToInterrupt() is preferred for portability.
    attachInterrupt(digitalPinToInterrupt(_tachPin), actual_ISR, RISING); // Or FALLING, depending on fan signal
}

/**
 * @brief Calculates and returns the fan's RPM.
 * Call this periodically (e.g., once per second).
 * @return Fan speed in RPM.
 */
unsigned int Tachometer::getRPM()
{
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - _lastReadTime;
    unsigned int rpm = 0;

    // Calculate RPM only if a significant time has passed (e.g., > 500ms)
    // to get a stable reading and avoid division by zero.
    if (elapsedTime > 500)
    {                   // Minimum interval for RPM calculation
        noInterrupts(); // Temporarily disable interrupts to safely read pulseCount
        unsigned long pulses = _pulseCount;
        _pulseCount = 0; // Reset pulse count for the next interval
        interrupts();    // Re-enable interrupts

        _lastReadTime = currentTime;

        // RPM = (Pulses / Pulses Per Revolution) / (Time in Minutes)
        // Time in Minutes = elapsedTime (ms) / 1000 (ms/s) / 60 (s/min)
        if (elapsedTime > 0)
        { // Avoid division by zero
            rpm = (pulses * 60000UL) / (PULSES_PER_REVOLUTION * elapsedTime);
        }
    }
    return rpm;
}