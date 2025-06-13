#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCDDisplay
{
public:
    // Constructor: Takes I2C address, columns, and rows
    LCDDisplay(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);

    void begin(); // Initializes the LCD
    void clear(); // Clears the LCD screen
    void displayData(float tempC, float humidity, float nh3_ppm, unsigned int fan_rpm);
    void printMessage(const String &line1, const String &line2 = "", const String &line3 = "", const String &line4 = "");
    void setCursor(uint8_t col, uint8_t row);
    void print(const String &message);

private:
    LiquidCrystal_I2C _lcd;
    uint8_t _cols;
    uint8_t _rows;

    // Helper to format float to string with specific precision
    String floatToString(float value, int decimalPlaces);
};

#endif // LCD_DISPLAY_H