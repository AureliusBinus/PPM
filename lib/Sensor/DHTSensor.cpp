// DHTSensor for TEMP and HUMID (formerly DHT22)

#include "DHTSensor.h" // Renamed from DHT22.h
#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE 22

DHTSensor::DHTSensor(int pin) : _dht(pin, DHTTYPE), _pin(pin) // Renamed from DHT22::DHT22
{
    // ...existing code...
}

void DHTSensor::begin() // Renamed from DHT22::begin
{
    _dht.begin();
    // ...existing code...
}

float DHTSensor::readTemperature(bool isFahrenheit) // Renamed from DHT22::readTemperature
{
    // ...existing code...
    float t = _dht.readTemperature(isFahrenheit);

    // ...existing code...
    if (isnan(t))
    {
        // ...existing code...
        return NAN;
    }
    return t;
}

float DHTSensor::readHumidity() // Renamed from DHT22::readHumidity
{
    // ...existing code...
    float h = _dht.readHumidity();

    // ...existing code...
    if (isnan(h))
    {
        // ...existing code...
        return NAN;
    }
    return h;
}

float DHTSensor::computeHeatIndex(float temperatureCelsius, float humidityPercent, bool isFahrenheit) // Renamed from DHT22::computeHeatIndex
{
    // ...existing code...
    float temp_c = temperatureCelsius;
    float hum_pct = humidityPercent;

    if (isnan(temp_c))
    {
        temp_c = readTemperature(false);
        if (isnan(temp_c))
            return NAN;
    }

    if (isnan(hum_pct))
    {
        hum_pct = readHumidity();
        if (isnan(hum_pct))
            return NAN;
    }

    // ...existing code...
    float temp_for_calc = temp_c;
    if (isFahrenheit)
    {
        temp_for_calc = _dht.convertCtoF(temp_c);
    }

    float hi = _dht.computeHeatIndex(temp_for_calc, hum_pct, isFahrenheit);
    return hi;
}