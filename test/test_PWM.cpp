#include <Arduino.h>
#include <unity.h>
#include "PWM.h" // Assuming PWM.h is in the include path (e.g., project's include folder)

// Define a dummy pin for testing if not already defined for the test environment
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

const int TEST_PWM_PIN = 9; // Example PWM pin

PWMFanControl fan(TEST_PWM_PIN);

void setUp(void) {
    // set up stuff, if needed
}

void tearDown(void) {
    // clean up stuff, if needed
    fan.off(); // Ensure fan is off after tests
}

void test_fan_constructor_and_begin(void) {
    // Simple test to ensure constructor and begin don't crash
    // In a real test environment for Arduino, pinMode would be called in begin()
    // For now, this is more of a compile and basic call test
    PWMFanControl localFan(TEST_PWM_PIN);
    localFan.begin();
    TEST_PASS_MESSAGE("Fan constructor and begin called.");
}

void test_fan_set_speed_percentage(void) {
    fan.begin(); // Ensure pinMode is called conceptually
    fan.setSpeed(0);    // Should call analogWrite(TEST_PWM_PIN, 0)
    fan.setSpeed(50);   // Should call analogWrite(TEST_PWM_PIN, 127 or 128)
    fan.setSpeed(100);  // Should call analogWrite(TEST_PWM_PIN, 255)
    // We can't easily check analogWrite calls without deeper mocking.
    // This test ensures the function can be called with various inputs.
    TEST_PASS_MESSAGE("Fan setSpeed called with 0, 50, 100%.");
}

void test_fan_set_speed_out_of_bounds(void) {
    fan.begin();
    fan.setSpeed(-10);  // Should be clamped to 0 -> analogWrite(TEST_PWM_PIN, 0)
    fan.setSpeed(110);  // Should be clamped to 100 -> analogWrite(TEST_PWM_PIN, 255)
    TEST_PASS_MESSAGE("Fan setSpeed called with out-of-bounds values.");
}

void test_fan_off(void) {
    fan.begin();
    fan.setSpeed(75);
    fan.off(); // Should call analogWrite(TEST_PWM_PIN, 0)
    TEST_PASS_MESSAGE("Fan off called.");
}

void test_fan_set_raw_duty_cycle(void) {
    fan.begin();
    fan.setRawDutyCycle(0);
    fan.setRawDutyCycle(128);
    fan.setRawDutyCycle(255);
    fan.setRawDutyCycle(-5);   // Clamped to 0
    fan.setRawDutyCycle(300);  // Clamped to 255
    TEST_PASS_MESSAGE("Fan setRawDutyCycle called with various values.");
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_fan_constructor_and_begin);
    RUN_TEST(test_fan_set_speed_percentage);
    RUN_TEST(test_fan_set_speed_out_of_bounds);
    RUN_TEST(test_fan_off);
    RUN_TEST(test_fan_set_raw_duty_cycle);
    UNITY_END();
}

void loop() {
    // Do nothing here
}