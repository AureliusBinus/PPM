#ifndef DHT22_H
#define DHT22_H

#include <DHT.h> // Add this include to make DHT type available

class DHT22
{
public:
    DHT22(int pin);
    void begin();
    float readTemperature(bool isFahrenheit = false);
    float readHumidity();
    float computeHeatIndex(float temperatureCelsius = NAN, float humidityPercent = NAN, bool isFahrenheit = false);

private:
    DHT _dht; // Declare _dht as type DHT
    int _pin;
};

#endif // DHT22_H