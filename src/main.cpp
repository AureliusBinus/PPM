// src/main.cpp
#include <Arduino.h>
#include "LCD.h"
#include "MQ135.h"
#include "DHTSensor.h" // Renamed from DHT22.h
#include "Tachometer.h"
#include "PWM.h"
#include "Buzzer.h" // Assuming path include/Buzzer.h

// --- Pin Definitions ---
// Sensors
#define MQ135_PIN A0
#define DHT22_PIN 4 // Changed D4 to 4
// Fan
#define FAN_PWM_PIN 9  // Changed D9 to 9
#define FAN_TACH_PIN 2 // Changed D2 to 2
// Alarms
#define ACTIVE_BUZZER_PIN 7  // Changed D7 to 7
#define PASSIVE_BUZZER_PIN 8 // Changed D8 to 8
// LCD (already defined)
#define LCD_ADDRESS 0x27
#define LCD_COLS 20 // Uncommented
#define LCD_ROWS 4  // Uncommented

// --- Danger Thresholds (Example Values) ---
#define HIGH_TEMP_THRESHOLD 35.0f
#define MEDIUM_TEMP_THRESHOLD 30.0f
#define HIGH_NH3_THRESHOLD 75.0f   // PPM
#define MEDIUM_NH3_THRESHOLD 30.0f // PPM

// --- Object Instances ---
LCDDisplay lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
MQ135 mq135(MQ135_PIN);
DHTSensor dht_sensor(DHT22_PIN); // Changed DHT22 to DHTSensor
Tachometer fanTach(FAN_TACH_PIN);
PWMFanControl fanControl(FAN_PWM_PIN);
ActiveBuzzer activeBuzzer(ACTIVE_BUZZER_PIN);
PassiveBuzzer passiveBuzzer(PASSIVE_BUZZER_PIN);

// State for alarms to prevent continuous re-triggering within the same state
enum DangerLevel
{
  SAFE,
  LOW_MEDIUM_DANGER,
  HIGH_DANGER
};
DangerLevel currentDangerLevel = SAFE;
DangerLevel previousDangerLevel = SAFE;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C bus for LCD

  lcd.begin();
  lcd.printMessage("System Booting...", "Sensor Init...");

  mq135.begin();
  // It's crucial to calibrate R0 for MQ135. For now, using default or a placeholder.
  // Consider adding a calibration routine or loading a saved R0 value.
  // Serial.println("Calibrating MQ135 R0 in clean air for 10 seconds...");
  // lcd.printMessage("Calibrating R0...", "Clean Air Please");
  // float sum_rs_calib = 0;
  // for(int i=0; i<10; i++) { sum_rs_calib += mq135.getRs(); delay(1000); }
  // mq135.setR0(sum_rs_calib / 10.0);
  // Serial.print("MQ135 R0 set to: "); Serial.println(mq135.getR0());
  // lcd.printMessage("R0 Calibrated:", String(mq135.getR0()));
  // delay(2000);

  dht_sensor.begin();
  fanTach.begin();
  fanControl.begin();
  activeBuzzer.begin();
  passiveBuzzer.begin();

  lcd.printMessage("System Ready.", "", "Monitoring...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  // Read sensor values
  float temp = dht_sensor.readTemperature();
  float humid = dht_sensor.readHumidity();
  float nh3 = mq135.readAmmoniaPPM();
  unsigned int rpm = fanTach.getRPM();

  // --- Dummy values for testing display & logic (remove or comment out for real use) ---
  // static int test_cycle = 0;
  // if (test_cycle == 0) { temp = 25.6f; humid = 55.2f; nh3 = 10.5f; rpm = 0; } // Safe
  // else if (test_cycle == 1) { temp = 32.0f; humid = 60.0f; nh3 = 40.0f; rpm = 1000; } // Medium
  // else if (test_cycle == 2) { temp = 37.0f; humid = 65.0f; nh3 = 80.0f; rpm = 2000; } // High
  // test_cycle = (test_cycle + 1) % 3;
  // --- End Dummy values ---

  // Check for valid readings (NaN)
  if (isnan(temp) || isnan(humid))
  {
    lcd.printMessage("DHT22 Error", "Check Sensor", "", "Retrying...");
    delay(2000);
    return; // Skip this loop iteration
  }
  if (nh3 < 0)
  { // Or some other error indicator from your MQ135 library if R0 is bad
    lcd.printMessage("MQ135 Error", "Check R0/Sensor", "", "Retrying...");
    // nh3 = 0; // Set to a safe value or handle error
    delay(2000);
    // return; // Decide if you want to halt or continue with potentially bad NH3
  }

  // Display data on LCD
  lcd.displayData(temp, humid, nh3, rpm);

  // Determine current danger level
  previousDangerLevel = currentDangerLevel; // Store last state

  if (temp > HIGH_TEMP_THRESHOLD || nh3 > HIGH_NH3_THRESHOLD)
  {
    currentDangerLevel = HIGH_DANGER;
  }
  else if (temp > MEDIUM_TEMP_THRESHOLD || nh3 > MEDIUM_NH3_THRESHOLD)
  {
    currentDangerLevel = LOW_MEDIUM_DANGER;
  }
  else
  {
    currentDangerLevel = SAFE;
  }

  // Fan control logic
  if (currentDangerLevel == HIGH_DANGER)
  {
    fanControl.setSpeed(100); // Max speed
  }
  else if (currentDangerLevel == LOW_MEDIUM_DANGER)
  {
    fanControl.setSpeed(60); // Medium speed
  }
  else
  {
    fanControl.setSpeed(0); // Fan off
  }

  // Alarm logic (act only on state change)
  if (currentDangerLevel != previousDangerLevel)
  {
    activeBuzzer.off();       // Ensure active is off before potentially starting passive
    passiveBuzzer.stopTone(); // Ensure passive is off

    switch (currentDangerLevel)
    {
    case HIGH_DANGER:
      Serial.println("HIGH DANGER DETECTED!");
      lcd.setCursor(0, 3);
      lcd.print("ALARM: HIGH DANGER  "); // Pad to clear previous
      activeBuzzer.beep(3, 300, 200);    // Beep 3 times: 300ms on, 200ms off
      break;
    case LOW_MEDIUM_DANGER:
      Serial.println("Medium/Low Danger Detected.");
      lcd.setCursor(0, 3);
      lcd.print("ALERT: Med/Low Risk");
      passiveBuzzer.playAlert(2, 2000, 150, 100); // 2 beeps, 2kHz, 150ms on, 100ms off
      break;
    case SAFE:
      Serial.println("Conditions Safe.");
      lcd.setCursor(0, 3);
      lcd.print("Status: All Clear  ");
      // Both buzzers already turned off
      break;
    }
  }
  else
  {
    // If in a continuous alarm state, you might want the alarm to repeat or be continuous
    // For example, if HIGH_DANGER persists, you might want activeBuzzer.on()
    // This example only beeps on state change. For continuous alarm:
    if (currentDangerLevel == HIGH_DANGER)
    {
      // activeBuzzer.on(); // This would make it continuous
      // Or, to repeat the beeps periodically while in high danger:
      // static unsigned long lastHighAlarmTime = 0;
      // if (millis() - lastHighAlarmTime > 5000) { // Repeat every 5 seconds
      //    activeBuzzer.beep(3, 300, 200);
      //    lastHighAlarmTime = millis();
      // }
    }
    else if (currentDangerLevel == LOW_MEDIUM_DANGER)
    {
      // static unsigned long lastMedAlarmTime = 0;
      // if (millis() - lastMedAlarmTime > 10000) { // Repeat every 10 seconds
      //    passiveBuzzer.playAlert(2, 2000, 150, 100);
      //    lastMedAlarmTime = millis();
      // }
    }
  }

  delay(2000); // Main loop delay
}