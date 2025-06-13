#include <Arduino.h>
#include <unity.h>
#include "Tachometer.h" // Assuming Tachometer.h is in the include path

const int TEST_TACH_PIN = 2; // Example Tach pin (must be an interrupt pin like D2 or D3 on Nano)

Tachometer tach(TEST_TACH_PIN);

void setUp(void)
{
    // set up stuff, if needed
}

void tearDown(void)
{
    // clean up stuff, if needed
    // detachInterrupt(digitalPinToInterrupt(TEST_TACH_PIN)); // Good practice if begin was called
}

void test_tach_constructor(void)
{
    Tachometer localTach(TEST_TACH_PIN);
    // This mainly tests that the constructor can be called.
    TEST_PASS_MESSAGE("Tachometer constructor called.");
}

void test_tach_begin(void)
{
    // This test ensures begin() can be called.
    // It would call pinMode and attachInterrupt.
    // Note: attachInterrupt might behave unexpectedly in a host-based test runner
    // if not running on actual hardware or a simulator that supports it.
    tach.begin();
    TEST_PASS_MESSAGE("Tachometer begin called.");
    // detachInterrupt(digitalPinToInterrupt(TEST_TACH_PIN)); // Clean up interrupt
}

void test_tach_get_rpm_initial(void)
{
    // tach.begin(); // Assuming begin was called in a previous test or setup
    unsigned int rpm = tach.getRPM();
    // Initially, or if not enough time has passed, RPM should be 0.
    TEST_ASSERT_EQUAL_UINT(0, rpm);
    // detachInterrupt(digitalPinToInterrupt(TEST_TACH_PIN));
}

// More complex RPM tests would require simulating ISR calls and time progression.
// For example, manually calling pulseDetected_ISR_Wrapper() and mocking millis().

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_tach_constructor);
    // Group begin and get_rpm_initial as begin sets up state for getRPM
    // For isolated tests, each test would handle its own begin/end if necessary.
    RUN_TEST(test_tach_begin); // Call begin before get_rpm_initial
    RUN_TEST(test_tach_get_rpm_initial);
    UNITY_END();
}

void loop()
{
}