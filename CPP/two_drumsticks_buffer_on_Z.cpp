#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

Adafruit_BNO055 bno1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(56, 0x29);

// Piezo
int sensoroutput = 0; 
int THRESHOLD = 70;
unsigned long lastPiezoTime = 0; // Timestamp of the last piezo trigger
const unsigned long piezoDebounce = 150; // Debounce time in milliseconds

const int BUFFER_SIZE = 4; // Easily change this to store more or fewer measurements

class SwingDetector {
  private:
    float history[BUFFER_SIZE];
    int currentIndex = 0;
    unsigned long lastPrintTime = 0;
    const unsigned long printDelay = 150; // Minimum delay between prints

  public:
    SwingDetector() {
      for (int i = 0; i < BUFFER_SIZE; i++) {
        history[i] = 0;
      }
    }

    void detect_swing(float currentAccel, int sensorID) {
      bool wentAbove10 = false;
      for (int i = 0; i < BUFFER_SIZE; i++) {
        if (history[i] > 10) {
          wentAbove10 = true;
          break;
        }
      }

      history[currentIndex] = currentAccel;
      currentIndex = (currentIndex + 1) % BUFFER_SIZE; // Update the index for next measurement

      unsigned long currentTime = millis();
      if (currentAccel < -14 && !wentAbove10 && (currentTime - lastPrintTime) > printDelay) {
        Serial.println(sensorID);
        lastPrintTime = currentTime;
      }
    }
};

SwingDetector detector1, detector2;

void setup() {
  Serial.begin(115200);
  Serial.println("IMU1 X Y Z - IMU2 X Y Z");

  if (!bno1.begin()) {
    Serial.print("No BNO055 detected at 0x28 ... Check your wiring or I2C ADDR!");
    while (1);
  }

  if (!bno2.begin()) {
    Serial.print("No BNO055 detected at 0x29 ... Check your wiring or I2C ADDR!");
    while (1);
  }
}

void loop() {
  int value = analogRead(sensoroutput);
  unsigned long currentTime = millis();

  // Debounce logic for piezo sensor
  if (value >= THRESHOLD && (currentTime - lastPiezoTime) > piezoDebounce) {
    Serial.println('4');
    lastPiezoTime = currentTime; // Update the last piezo trigger time
    delay(150); // Additional delay to ensure debounce time is respected
  }

  imu::Vector<3> acceleration1 = bno1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float accelZ1 = acceleration1.z();
  detector1.detect_swing(accelZ1, 0);

  imu::Vector<3> acceleration2 = bno2.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float accelZ2 = acceleration2.z();
  detector2.detect_swing(accelZ2, 3);

  delay(20);
}
