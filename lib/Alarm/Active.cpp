// 5v Mini Buzzer active
#include "Buzzer.h"

ActiveBuzzer::ActiveBuzzer(int pin) : _pin(pin), _isOn(false) {}

void ActiveBuzzer::begin()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW); // Start with buzzer off
    _isOn = false;
}

void ActiveBuzzer::on()
{
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void ActiveBuzzer::off()
{
    digitalWrite(_pin, LOW);
    _isOn = false;
}

void ActiveBuzzer::beep(unsigned int duration_ms)
{
    digitalWrite(_pin, HIGH);
    _isOn = true;
    delay(duration_ms);
    digitalWrite(_pin, LOW);
    _isOn = false;
}

void ActiveBuzzer::beep(unsigned int count, unsigned int on_duration_ms, unsigned int off_duration_ms)
{
    for (unsigned int i = 0; i < count; ++i)
    {
        digitalWrite(_pin, HIGH);
        _isOn = true; // Technically true during the on phase
        delay(on_duration_ms);
        digitalWrite(_pin, LOW);
        _isOn = false;
        if (i < count - 1)
        { // Don't delay after the last beep
            delay(off_duration_ms);
        }
    }
}