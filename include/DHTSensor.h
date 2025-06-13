#ifndef DHTSENSOR_H // Renamed from DHT22_H
#define DHTSENSOR_H // Renamed from DHT22_H

#include <DHT.h>

class DHTSensor // Renamed from DHT22
{
public:
    DHTSensor(int pin); // Renamed from DHT22
    void begin();
    float readTemperature(bool isFahrenheit = false);
    float readHumidity();
    float computeHeatIndex(float temperatureCelsius = NAN, float humidityPercent = NAN, bool isFahrenheit = false);

private:
    DHT _dht;
    int _pin;
};

#endif // DHTSENSOR_H // Renamed from DHT22_H