// LCD i2c 20x4
#include "LCD.h"

// Common I2C LCD addresses are 0x27 or 0x3F.
// You might need to use an I2C scanner sketch to find yours if unsure.
LCDDisplay::LCDDisplay(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_Addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows) {}

void LCDDisplay::begin()
{
    _lcd.init();      // Initialize the lcd
    _lcd.backlight(); // Turn on the backlight
    _lcd.clear();
}

void LCDDisplay::clear()
{
    _lcd.clear();
}

void LCDDisplay::setCursor(uint8_t col, uint8_t row)
{
    _lcd.setCursor(col, row);
}

void LCDDisplay::print(const String &message)
{
    _lcd.print(message);
}

String LCDDisplay::floatToString(float value, int decimalPlaces)
{
    char buffer[10]; // Adjust size as needed
    dtostrf(value, 0, decimalPlaces, buffer);
    return String(buffer);
}

void LCDDisplay::displayData(float tempC, float humidity, float nh3_ppm, unsigned int fan_rpm)
{
    _lcd.clear();

    // Line 0: Temperature and Humidity
    _lcd.setCursor(0, 0);
    _lcd.print("T:");
    _lcd.print(floatToString(tempC, 1));
    _lcd.print((char)223); // Degree symbol
    _lcd.print("C ");
    _lcd.setCursor(10, 0); // Adjust spacing as needed for 20 cols
    _lcd.print("H:");
    _lcd.print(floatToString(humidity, 1));
    _lcd.print("%");

    // Line 1: NH3 PPM
    _lcd.setCursor(0, 1);
    _lcd.print("NH3: ");
    _lcd.print(floatToString(nh3_ppm, 1));
    _lcd.print(" PPM");

    // Line 2: Fan RPM
    _lcd.setCursor(0, 2);
    _lcd.print("Fan RPM: ");
    String rpmStr = String(fan_rpm);
    // Pad with spaces if RPM is short to overwrite previous longer numbers
    while (rpmStr.length() < 5)
    {
        rpmStr = " " + rpmStr;
    }
    _lcd.print(rpmStr);

    // Line 3: Can be used for status or other info
    // _lcd.setCursor(0, 3);
    // _lcd.print("Status: OK");
}

void LCDDisplay::printMessage(const String &line1, const String &line2, const String &line3, const String &line4)
{
    _lcd.clear();
    if (line1.length() > 0)
    {
        _lcd.setCursor(0, 0);
        _lcd.print(line1.substring(0, _cols));
    }
    if (line2.length() > 0 && _rows > 1)
    {
        _lcd.setCursor(0, 1);
        _lcd.print(line2.substring(0, _cols));
    }
    if (line3.length() > 0 && _rows > 2)
    {
        _lcd.setCursor(0, 2);
        _lcd.print(line3.substring(0, _cols));
    }
    if (line4.length() > 0 && _rows > 3)
    {
        _lcd.setCursor(0, 3);
        _lcd.print(line4.substring(0, _cols));
    }
}