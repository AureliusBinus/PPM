#include <Arduino.h>
#include <unity.h>
#include "LCD.h"  // Assuming LCD.h is in the include path
#include <Wire.h> // LCD library depends on Wire

// Define LCD properties for testing
#define TEST_LCD_ADDRESS 0x27
#define TEST_LCD_COLS 20
#define TEST_LCD_ROWS 4

LCDDisplay lcd(TEST_LCD_ADDRESS, TEST_LCD_COLS, TEST_LCD_ROWS);

void setUp(void)
{
    // Wire.begin(); // Important for I2C communication
    // lcd.begin(); // Initialize LCD for each test if needed, or once in setup
}

void tearDown(void)
{
    // lcd.clear();
}

void test_lcd_constructor_and_begin(void)
{
    // Tests if constructor and begin can be called without crashing.
    // Actual I2C communication won't be verified here without hardware/mock.
    LCDDisplay localLcd(TEST_LCD_ADDRESS, TEST_LCD_COLS, TEST_LCD_ROWS);
    Wire.begin(); // Simulate Wire init
    localLcd.begin();
    TEST_PASS_MESSAGE("LCD constructor and begin called.");
}

void test_lcd_print_message(void)
{
    Wire.begin();
    lcd.begin(); // Ensure initialized
    lcd.printMessage("Line 1 test", "Line 2 test", "Line 3 test", "Line 4 test");
    // This is a smoke test; we can't verify actual display content easily.
    TEST_PASS_MESSAGE("LCD printMessage called.");
}

void test_lcd_display_data(void)
{
    Wire.begin();
    lcd.begin(); // Ensure initialized
    lcd.displayData(25.5f, 60.2f, 15.7f, 1500);
    TEST_PASS_MESSAGE("LCD displayData called.");
}

void test_lcd_clear(void)
{
    Wire.begin();
    lcd.begin(); // Ensure initialized
    lcd.clear();
    TEST_PASS_MESSAGE("LCD clear called.");
}

void test_lcd_set_cursor_and_print(void)
{
    Wire.begin();
    lcd.begin(); // Ensure initialized
    lcd.setCursor(5, 1);
    lcd.print("Test");
    TEST_PASS_MESSAGE("LCD setCursor and print called.");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    // It's good practice to initialize Wire once if multiple tests use I2C
    // Wire.begin(); // Moved to individual tests or ensure it's called before lcd.begin()

    RUN_TEST(test_lcd_constructor_and_begin);
    RUN_TEST(test_lcd_print_message);
    RUN_TEST(test_lcd_display_data);
    RUN_TEST(test_lcd_clear);
    RUN_TEST(test_lcd_set_cursor_and_print);
    UNITY_END();
}

void loop() {}