// 5v Mini Buzzer passive
#include "Buzzer.h"

PassiveBuzzer::PassiveBuzzer(int pin) : _pin(pin) {}

void PassiveBuzzer::begin()
{
    pinMode(_pin, OUTPUT);
    // No initial tone
}

void PassiveBuzzer::playTone(unsigned int frequency, unsigned long duration_ms)
{
    if (duration_ms > 0)
    {
        tone(_pin, frequency, duration_ms);
        // tone() with duration handles stopping automatically after duration.
        // A small delay might be needed if immediately followed by another command
        // that could interfere, but usually not for simple beeps.
    }
    else
    {
        tone(_pin, frequency); // Plays indefinitely until stopTone() is called
    }
}

void PassiveBuzzer::stopTone()
{
    noTone(_pin);
}

void PassiveBuzzer::playAlert(unsigned int count, unsigned int frequency, unsigned int on_duration_ms, unsigned int off_duration_ms)
{
    for (unsigned int i = 0; i < count; ++i)
    {
        tone(_pin, frequency, on_duration_ms);
        // The tone function with duration will play for on_duration_ms.
        // We need to wait for that duration plus the off_duration_ms before the next tone.
        delay(on_duration_ms); // Wait for the tone to finish
        if (i < count - 1)
        {                           // Don't delay after the last beep's sound
            delay(off_duration_ms); // Pause between beeps
        }
    }
    // Ensure the buzzer is off after the sequence if it was a continuous tone before,
    // though tone() with duration handles this. If playTone(freq, 0) was used prior,
    // this explicit stopTone() might be redundant but safe.
    // noTone(_pin); // Generally not needed here as tone() with duration stops itself.
}