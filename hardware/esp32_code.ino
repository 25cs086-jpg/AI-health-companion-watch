/*
  AI-Powered Personal Health Companion Watch
  ESP32 Hackathon Prototype

  Libraries:
    - Wire (built-in)
    - Adafruit MPU6050
    - Adafruit Unified Sensor
    - MAX30105 (SparkFun MAX3010x library)

  Install libraries from Arduino IDE Library Manager.

  IMPORTANT:
  This is a prototype. Heart-rate calculation and fall thresholds
  should be calibrated for your actual sensors and enclosure.
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"

Adafruit_MPU6050 mpu;
MAX30105 particleSensor;

const int BUTTON_PIN = 27;
const int VIBRATION_PIN = 26;
const int BUZZER_PIN = 25;

const float FALL_ACCEL_G = 2.7;       // Prototype threshold
const unsigned long RESPONSE_WINDOW_MS = 10000;

bool fallPending = false;
unsigned long fallTime = 0;
unsigned long lastPrint = 0;

void pulseAlert() {
  digitalWrite(VIBRATION_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(VIBRATION_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void printTelemetry(float bpm, float accelG, const char* status,
                    bool fall, bool emergency) {
  Serial.print("{\"heartRate\":");
  Serial.print(bpm, 1);
  Serial.print(",\"accelG\":");
  Serial.print(accelG, 2);
  Serial.print(",\"status\":\"");
  Serial.print(status);
  Serial.print("\",\"fallDetected\":");
  Serial.print(fall ? "true" : "false");
  Serial.print(",\"emergency\":");
  Serial.print(emergency ? "true" : "false");
  Serial.println("}");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(VIBRATION_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(VIBRATION_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("ERROR: MPU6050 not detected.");
    while (true) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      delay(850);
    }
  }

  // Configure MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Configure MAX30102/MAX30105 family sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("ERROR: MAX30102/MAX30105 sensor not detected.");
    while (true) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(400);
    }
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("AI Health Companion Watch ready.");
}

void loop() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float az = accel.acceleration.z;

  float accelMagnitude = sqrt(ax * ax + ay * ay + az * az);
  float accelG = accelMagnitude / 9.80665;

  long irValue = particleSensor.getIR();

  // This is only a demo approximation.
  // Replace with a validated beat-detection algorithm for real use.
  float bpm = 0.0;
  if (irValue > 50000) {
    bpm = 75.0; // placeholder demo value
  }

  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

  if (!fallPending && accelG >= FALL_ACCEL_G) {
    fallPending = true;
    fallTime = millis();

    pulseAlert();

    Serial.println(
      "{\"event\":\"FALL_DETECTED\",\"message\":\"Are you okay?\"}"
    );
  }

  bool emergency = false;

  if (fallPending) {
    // Button press means the user responded.
    if (buttonPressed) {
      fallPending = false;
      Serial.println(
        "{\"event\":\"USER_OK\",\"message\":\"User confirmed safety.\"}"
      );
    }
    else if (millis() - fallTime >= RESPONSE_WINDOW_MS) {
      fallPending = false;
      emergency = true;

      Serial.println(
        "{\"event\":\"EMERGENCY\",\"message\":\"Guardian alert required.\"}"
      );

      // In a production design, trigger your approved SMS/call service here.
      pulseAlert();
    }
  }

  if (millis() - lastPrint >= 2000) {
    lastPrint = millis();

    const char* status = (bpm > 0 && bpm < 120)
                           ? "NORMAL"
                           : "CHECK";

    printTelemetry(
      bpm,
      accelG,
      status,
      fallPending,
      emergency
    );
  }

  delay(20);
}
