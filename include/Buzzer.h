#ifndef ACTIVE_BUZZER_H
#define ACTIVE_BUZZER_H

#include <Arduino.h>

class ActiveBuzzer
{
public:
    ActiveBuzzer(int pin);
    void begin();
    void on();
    void off();
    void beep(unsigned int duration_ms);
    void beep(unsigned int count, unsigned int on_duration_ms, unsigned int off_duration_ms); // Beep multiple times

private:
    int _pin;
    bool _isOn; // To track state if needed, though simple on/off might not require it
};

#endif // ACTIVE_BUZZER_H

#ifndef PASSIVE_BUZZER_H
#define PASSIVE_BUZZER_H

#include <Arduino.h>

class PassiveBuzzer
{
public:
    PassiveBuzzer(int pin);
    void begin();
    void playTone(unsigned int frequency, unsigned long duration_ms = 0); // duration 0 for continuous until stopTone()
    void stopTone();
    void playAlert(unsigned int count = 3, unsigned int frequency = 1500, unsigned int on_duration_ms = 100, unsigned int off_duration_ms = 100);

private:
    int _pin;
};

#endif // PASSIVE_BUZZER_H