#include <Arduino.h>
#include <unity.h>
#include "Buzzer.h" // Contains both ActiveBuzzer and PassiveBuzzer

const int TEST_ACTIVE_BUZZER_PIN = 7;
const int TEST_PASSIVE_BUZZER_PIN = 8;

ActiveBuzzer activeBuzzer(TEST_ACTIVE_BUZZER_PIN);
PassiveBuzzer passiveBuzzer(TEST_PASSIVE_BUZZER_PIN);

void setUp(void)
{
    // activeBuzzer.begin(); // Call begin if state needs to be reset for each test
    // passiveBuzzer.begin();
}

void tearDown(void)
{
    activeBuzzer.off();
    passiveBuzzer.stopTone();
}

// --- Active Buzzer Tests ---
void test_active_buzzer_constructor_and_begin(void)
{
    ActiveBuzzer localBuzzer(TEST_ACTIVE_BUZZER_PIN);
    localBuzzer.begin(); // Calls pinMode, digitalWrite
    TEST_PASS_MESSAGE("ActiveBuzzer constructor and begin called.");
}

void test_active_buzzer_on_off(void)
{
    activeBuzzer.begin();
    activeBuzzer.on();  // digitalWrite HIGH
    activeBuzzer.off(); // digitalWrite LOW
    TEST_PASS_MESSAGE("ActiveBuzzer on and off called.");
}

void test_active_buzzer_beep_single(void)
{
    activeBuzzer.begin();
    activeBuzzer.beep(100); // Calls digitalWrite, delay, digitalWrite
    TEST_PASS_MESSAGE("ActiveBuzzer single beep called.");
}

void test_active_buzzer_beep_multiple(void)
{
    activeBuzzer.begin();
    activeBuzzer.beep(3, 50, 20); // Loop with digitalWrite and delays
    TEST_PASS_MESSAGE("ActiveBuzzer multiple beeps called.");
}

// --- Passive Buzzer Tests ---
void test_passive_buzzer_constructor_and_begin(void)
{
    PassiveBuzzer localBuzzer(TEST_PASSIVE_BUZZER_PIN);
    localBuzzer.begin(); // Calls pinMode
    TEST_PASS_MESSAGE("PassiveBuzzer constructor and begin called.");
}

void test_passive_buzzer_play_tone_with_duration(void)
{
    passiveBuzzer.begin();
    passiveBuzzer.playTone(1000, 100); // Calls tone(pin, freq, duration)
    delay(110);                        // Allow tone to finish for test sequencing
    TEST_PASS_MESSAGE("PassiveBuzzer playTone with duration called.");
}

void test_passive_buzzer_play_tone_continuous_and_stop(void)
{
    passiveBuzzer.begin();
    passiveBuzzer.playTone(1500); // Calls tone(pin, freq)
    // In a real scenario, sound would play. Here, we just check the call.
    passiveBuzzer.stopTone(); // Calls noTone(pin)
    TEST_PASS_MESSAGE("PassiveBuzzer continuous playTone and stopTone called.");
}

void test_passive_buzzer_play_alert(void)
{
    passiveBuzzer.begin();
    passiveBuzzer.playAlert(2, 2000, 50, 30); // Loop with tone() and delays
    delay((50 + 30) * 2 + 50);                // Allow alert to finish
    TEST_PASS_MESSAGE("PassiveBuzzer playAlert called.");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    // Active Buzzer
    RUN_TEST(test_active_buzzer_constructor_and_begin);
    RUN_TEST(test_active_buzzer_on_off);
    RUN_TEST(test_active_buzzer_beep_single);
    RUN_TEST(test_active_buzzer_beep_multiple);

    // Passive Buzzer
    RUN_TEST(test_passive_buzzer_constructor_and_begin);
    RUN_TEST(test_passive_buzzer_play_tone_with_duration);
    RUN_TEST(test_passive_buzzer_play_tone_continuous_and_stop);
    RUN_TEST(test_passive_buzzer_play_alert);

    UNITY_END();
}

void loop()
{
}