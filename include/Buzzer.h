#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class ActiveBuzzer
{
public:
    ActiveBuzzer(int pin);
    void begin();
    void on();
    void off();
    void beep(unsigned int duration_ms);
    void beep(unsigned int count, unsigned int on_duration_ms, unsigned int off_duration_ms);

private:
    int _pin;
    bool _isOn; // To track state
};

class PassiveBuzzer
{
public:
    PassiveBuzzer(int pin);
    void begin();
    void playTone(unsigned int frequency, unsigned long duration_ms = 0); // duration 0 for continuous
    void stopTone();
    void playAlert(unsigned int count = 3, unsigned int frequency = 1500, unsigned int on_duration_ms = 100, unsigned int off_duration_ms = 100);

private:
    int _pin;
};

#endif // BUZZER_H