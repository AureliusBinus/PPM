#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h> // For DHT object type, and NAN if used in function signatures

class DHTSensor
{
public:
    DHTSensor(int pin);
    void begin();
    float readTemperature(bool isFahrenheit = false);
    float readHumidity();
    float computeHeatIndex(float temperatureCelsius = NAN, float humidityPercent = NAN, bool isFahrenheit = false);

private:
    DHT _dht; // DHT sensor object from Adafruit library
    int _pin; // Store the pin number
};

#endif // DHTSENSOR_H