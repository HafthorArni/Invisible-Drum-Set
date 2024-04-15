#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Arduino.h>

// Define two BNO055 instances with different I2C addresses
Adafruit_BNO055 bno1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(56, 0x29);

int sensoroutput = 0; // the analog pin connected to the sensor
int ledoutput = 0; // pin connected to LED
int THRESHOLD = 100;

void setup() {
  Serial.begin(115200);
  Serial.println("IMU1 X Y Z - IMU2 X Y Z");

  // Initialize the first BNO055
  if (!bno1.begin()) {
    Serial.print("No BNO055 detected at 0x28 ... Check your wiring or I2C ADDR!");
    while (1);
  }
  //bno1.setMode(0x0C); // Set the mode to NDOF

  // Initialize the second BNO055
  if (!bno2.begin()) {
    Serial.print("No BNO055 detected at 0x29 ... Check your wiring or I2C ADDR!");
    while (1);
  }
  //bno2.setMode(0x0C); // Set the mode to NDOF
}

void loop() {

  int value = analogRead(sensoroutput);  // function to read analog voltage from sensor
  if (value >= THRESHOLD)                    // function to check voltage level from sensor
  {
  Serial.println('4');
  delay(150);
  }
  // Read orientation and acceleration data from the first IMU
  imu::Vector<3> euler1 = bno1.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> acceleration1 = bno1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float accelX1 = acceleration1.x();
  float heading1 = euler1.x();
  float pitch1 = euler1.y();

    // Read orientation and acceleration data from the second IMU
  imu::Vector<3> euler2 = bno2.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> acceleration2 = bno2.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float accelX2 = acceleration2.x();
  float heading2 = euler2.x();
  float pitch2 = euler2.y();

  if (accelX1 > 25) {
    if (0 < heading1 && heading1 < 180) {
      if (pitch1 < -40) {
        Serial.println("2");
      } else {
        Serial.println("1");
      }
    } else if (180 < heading1 && heading1 < 360) {
      if (pitch1 < -40) {
        Serial.println("3");
      } else {
        Serial.println("0");
      }
    }
  }

  if (accelX2 > 25) {
    if (0 < heading2 && heading2 < 180) {
      if (pitch2 < -40) {
        Serial.println("2");
      } else {
        Serial.println("1");
      }
    } else if (180 < heading2 && heading2 < 360) {
      if (pitch2 < -40) {
        Serial.println("3");
      } else {
        Serial.println("0");
      }
    }
  }

  

  // Delay before the next reading
  delay(20);
}
