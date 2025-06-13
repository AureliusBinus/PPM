// MQ135 Gas Sensor for Ammonia (NH3)

#include "MQ135.h"   // Assuming MQ135.h declares the MQ135 class
#include <Arduino.h> // For analogRead, delay, etc.
#include <math.h>    // For pow function

// --- Sensor and Environment Configuration ---
// Default R0 value in clean air. This value MUST be calibrated for your specific sensor.
// To calibrate: run the sensor in clean air for an extended period (e.g., 12-24 hours burn-in, then a few hours for stabilization),
// then call calibrateR0() or read the Rs value using getRs() and set it via setR0().
#define DEFAULT_R0 76.65f // Example value, replace with your calibrated R0.

// Load resistor value on your MQ135 breakout board (typically 10kOhms or as specified).
#define RL_VALUE 10.0f // kOhms

// --- Ammonia (NH3) Curve Parameters ---
// These parameters are for the formula: PPM = A * (Rs/R0)^B
// These are generic values derived from typical datasheets. For best accuracy,
// consult your sensor's specific datasheet or perform a multi-point calibration.
#define AMMONIA_CURVE_A 63.69f  // Parameter 'A' for NH3
#define AMMONIA_CURVE_B -2.021f // Parameter 'B' for NH3

// --- ADC and Voltage Configuration ---
// VCC voltage supplied to the sensor and used as ADC reference.
// Common values: 5.0V for Arduino Uno/Mega, 3.3V for ESP32/ESP8266.
// Ensure this matches your board's logic level and ADC reference.
#define VCC_VOLTAGE 5.0f

// ADC resolution of your microcontroller.
#define ADC_MAX_VALUE 1023.0f // For 10-bit ADC (e.g., Arduino Uno, Mega)
// #define ADC_MAX_VALUE 4095.0f // For 12-bit ADC (e.g., ESP32)

MQ135::MQ135(int analogPin)
{
    _analogPin = analogPin;
    _R0 = DEFAULT_R0; // Initialize R0 with a default; calibration is crucial.
}

void MQ135::begin()
{
    // pinMode(_analogPin, INPUT); // Generally not required for analogRead on Arduino
    // You can add a burn-in delay here if desired, though burn-in is best done externally.
}

/**
 * @brief Reads the raw ADC value from the sensor's analog output.
 * @return The raw ADC value (e.g., 0-1023 for a 10-bit ADC).
 */
int MQ135::readRawADC()
{
    return analogRead(_analogPin);
}

/**
 * @brief Calculates the sensor's resistance (Rs) in kOhms.
 * This value changes based on gas concentration.
 * @return The sensor resistance (Rs) in kOhms.
 */
float MQ135::getRs()
{
    int adc_raw = readRawADC();
    float sensor_voltage_rl = (static_cast<float>(adc_raw) / ADC_MAX_VALUE) * VCC_VOLTAGE;

    // Prevent division by zero or invalid calculations if sensor_voltage_rl is at extremes.
    // If sensor_voltage_rl is very low (high Rs), cap it to avoid infinite Rs.
    if (sensor_voltage_rl < 0.001f)
    {
        sensor_voltage_rl = 0.001f;
    }
    // If sensor_voltage_rl is very high (low Rs, near VCC), cap it to avoid negative or zero Rs.
    if (sensor_voltage_rl > (VCC_VOLTAGE - 0.001f))
    {
        sensor_voltage_rl = VCC_VOLTAGE - 0.001f;
    }

    // Calculate Rs using the voltage divider formula: Rs = RL * (VCC - V_RL) / V_RL
    float Rs = RL_VALUE * (VCC_VOLTAGE - sensor_voltage_rl) / sensor_voltage_rl;
    return Rs;
}

/**
 * @brief Calibrates the R0 value (sensor resistance in clean air).
 * Run this method when the sensor is in a known clean air environment
 * after sufficient burn-in and stabilization time.
 * @param rs_in_clean_air Optional: directly provide an Rs value measured in clean air.
 *                        If 0 (default), it will take multiple internal readings and average them.
 */
void MQ135::calibrateR0(float rs_in_clean_air)
{
    if (rs_in_clean_air > 0.0f)
    {
        _R0 = rs_in_clean_air;
    }
    else
    {
        // Perform an averaged reading for R0 calibration
        float sum_rs = 0.0f;
        const int num_samples = 20; // Number of samples to average for calibration
        // Serial.println("Calibrating R0. Ensure sensor is in clean air."); // Debug
        for (int i = 0; i < num_samples; i++)
        {
            sum_rs += getRs();
            delay(100); // Short delay between samples
        }
        _R0 = sum_rs / static_cast<float>(num_samples);
    }
    // Serial.print("MQ135 R0 calibrated to: "); Serial.println(_R0); // Debug
}

/**
 * @brief Sets a specific R0 value. Useful if R0 is known from previous calibration.
 * @param r0_value The R0 value (resistance in clean air) to set.
 */
void MQ135::setR0(float r0_value)
{
    if (r0_value > 0.0f)
    {
        _R0 = r0_value;
    }
}

/**
 * @brief Gets the current R0 value being used by the sensor.
 * @return The R0 value in kOhms.
 */
float MQ135::getR0()
{
    return _R0;
}

/**
 * @brief Reads the concentration of Ammonia (NH3) in Parts Per Million (PPM).
 * Ensure R0 is properly calibrated for accurate readings.
 * @return Ammonia concentration in PPM. Returns 0.0f if R0 is not valid or readings are problematic.
 */
float MQ135::readAmmoniaPPM()
{
    if (_R0 <= 0.0f)
    {
        // Serial.println("Error: R0 is not calibrated or set to a valid value."); // Debug
        return 0.0f; // R0 must be positive and calibrated.
    }

    float rs = getRs();
    if (rs < 0.0f)
    { // Should ideally not happen due to getRs() guards
        // Serial.println("Error: Invalid Rs value."); // Debug
        return 0.0f;
    }

    float ratio = rs / _R0;
    if (ratio <= 0.0f)
    { // Ratio must be positive for the pow function.
        // Serial.println("Error: Invalid Rs/R0 ratio."); // Debug
        return 0.0f;
    }

    // Calculate PPM using the formula: PPM = A * (Rs/R0)^B
    float ppm = AMMONIA_CURVE_A * pow(ratio, AMMONIA_CURVE_B);
    return ppm;
}

/**
 * @brief Gets the current Rs/R0 ratio.
 * This can be useful for debugging or applying custom gas calculation curves
 * or temperature/humidity corrections.
 * @return The Rs/R0 ratio. Returns 0.0f if R0 is not valid.
 */
float MQ135::getRsR0Ratio()
{
    if (_R0 <= 0.0f)
    {
        return 0.0f;
    }
    float rs = getRs();
    if (rs < 0.0f)
    {
        return 0.0f;
    }
    return rs / _R0;
}