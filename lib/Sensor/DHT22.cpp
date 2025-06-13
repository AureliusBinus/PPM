// DHT22 for TEMP and HUMID

#include "DHT22.h"   // Assuming DHT22.h declares the DHT22 class
#include <Arduino.h> // For basic Arduino functions (not strictly needed for DHT library usage itself but good practice)
#include <DHT.h>     // Adafruit DHT Sensor Library

// Define the type of DHT sensor
#define DHTTYPE 22 // We are using DHT22

DHT22::DHT22(int pin) : _dht(pin, DHTTYPE), _pin(pin)
{
    // The constructor for the DHT object (_dht) is called in the initializer list.
    // _pin is stored for reference if needed, though _dht handles direct pin interaction.
}

void DHT22::begin()
{
    _dht.begin();
    // You might want to add a small delay here to allow the sensor to stabilize,
    // though the library often handles initial read delays.
    // delay(2000); // Example: wait 2 seconds for sensor to stabilize
}

/**
 * @brief Reads the temperature from the DHT22 sensor.
 * @param isFahrenheit Set to true to get temperature in Fahrenheit, false for Celsius (default).
 * @return Temperature value. Returns NAN (Not A Number) if the read fails.
 */
float DHT22::readTemperature(bool isFahrenheit)
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float t = _dht.readTemperature(isFahrenheit);

    // Check if any reads failed and exit early (to try again).
    if (isnan(t))
    {
        // Serial.println(F("Failed to read temperature from DHT sensor!")); // Optional debug
        return NAN;
    }
    return t;
}

/**
 * @brief Reads the humidity from the DHT22 sensor.
 * @return Humidity value (%). Returns NAN (Not A Number) if the read fails.
 */
float DHT22::readHumidity()
{
    float h = _dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h))
    {
        // Serial.println(F("Failed to read humidity from DHT sensor!")); // Optional debug
        return NAN;
    }
    return h;
}

/**
 * @brief Reads both temperature and humidity to ensure a fresh reading for both,
 *        then computes the heat index.
 * @param temperatureCelsius Current temperature in Celsius (will be read if NAN).
 * @param humidityPercent Current humidity in percent (will be read if NAN).
 * @param isFahrenheit Set to true to compute heat index in Fahrenheit.
 * @return Heat index value. Returns NAN if temperature or humidity read fails.
 */
float DHT22::computeHeatIndex(float temperatureCelsius, float humidityPercent, bool isFahrenheit)
{
    float temp_c = temperatureCelsius;
    float hum_pct = humidityPercent;

    if (isnan(temp_c))
    {
        temp_c = readTemperature(false); // Read in Celsius
        if (isnan(temp_c))
            return NAN; // Failed to get temperature
    }

    if (isnan(hum_pct))
    {
        hum_pct = readHumidity();
        if (isnan(hum_pct))
            return NAN; // Failed to get humidity
    }

    // If the user wants Fahrenheit, and we have Celsius, convert the input temp for the calculation
    // The Adafruit library's computeHeatIndex takes temperature in the desired output unit.
    float temp_for_calc = temp_c;
    if (isFahrenheit)
    {
        temp_for_calc = _dht.convertCtoF(temp_c);
    }

    float hi = _dht.computeHeatIndex(temp_for_calc, hum_pct, isFahrenheit);
    return hi;
}
