#ifndef MQ135_H
#define MQ135_H

class MQ135
{
public:
    MQ135(int analogPin);
    void begin();
    float getRs();
    void calibrateR0(float rs_in_clean_air = 0.0f);
    void setR0(float r0_value);
    float getR0();
    float readAmmoniaPPM();
    float getRsR0Ratio();
    int readRawADC();

private:
    int _analogPin;
    float _R0;
};

#endif // MQ135_H