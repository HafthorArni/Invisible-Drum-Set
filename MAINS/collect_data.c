#ifndef F_CPU
#define F_CPU 16000000UL // Standard Arduino clock speed
#endif

#include "serial.h"
#include "bno055.h"
#include "timer.h"
#include "digital_in.h"
#include <util/delay.h>
#include <stdbool.h>

#define address1 0x28 // I2C address of the BNO055 sensor
#define address2 0x29 // I2C address of the BNO055 sensor

// Variables for debouncing first button
#define DEBOUNCE_DELAY1 50 // Debounce delay in milliseconds
unsigned long lastDebounceTime1 = 0; // the last time the output pin was toggled
uint8_t buttonprinted1 = false; // Initialize to false
// first button pin
#define button1 6


// Function to enable internal pullups on SDA and SCL lines
void enableInternalPullups(void) {
    // Set SDA and SCL as input
    DDRC &= ~((1 << DDC4) | (1 << DDC5));
    // Enable pull-up resistors on SDA and SCL lines
    PORTC |= (1 << PORTC4) | (1 << PORTC5);
}

int main(void) {
    enableInternalPullups(); // Enable internal pullups
    SerialBegin(115200); // Initialize serial communication
    init_millis(F_CPU);  // Initialize millis using the custom time library
    digital_in_init(button1);  // Initialize digital input on pin 6
    bno055_init(address1);       // Initialize the first BNO055 sensor
    bno055_init(address2);       // Initialize the second BNO055 sensor


    while (1) {
        // acceleration data variables
        int16_t accel1_z; // Acceleration data variables
        int16_t accel2_z; // Acceleration data variables
        // quaternion data variables
        float quat1_w, quat1_x, quat1_y, quat1_z; // Quaternion data variables
        float quat2_w, quat2_x, quat2_y, quat2_z; // Quaternion data variables
        // euler data variables
        float roll1, pitch1, yaw1; // Euler data variables
        float roll2, pitch2, yaw2; // Euler data variables
        // gyro data variables
        int16_t gyro1_x, gyro1_y, gyro1_z; // Gyro data variables
        int16_t gyro2_x, gyro2_y, gyro2_z; // Gyro data variables
        
        // debounce the first button print first imu if it goes from low to high
        if (millis() - lastDebounceTime1 > DEBOUNCE_DELAY1) {
            uint8_t reading = digital_in_read(button1);  // Read the button state

            if (reading == 1 && !buttonprinted1) {
                buttonprinted1 = true;
                }
            if (reading == 0) {
                buttonprinted1 = false;
                }
            lastDebounceTime1 = millis();
            }

        // if button 1 is pressed print accel1_z for 5 seconds
        if (buttonprinted1) {
            //SerialPrintStr("\nstart\n");
            unsigned long startTime = millis();
            while (millis() - startTime < 5000) {
                bno055_read_accelerometer_z(address1, &accel1_z);
                bno055_read_accelerometer_z(address2, &accel2_z);
                bno055_read_quaternion(address1, &quat1_w, &quat1_x, &quat1_y, &quat1_z);
                bno055_read_quaternion(address2, &quat2_w, &quat2_x, &quat2_y, &quat2_z);
                bno055_quaternion_to_euler(quat1_w, quat1_x, quat1_y, quat1_z, &roll1, &pitch1, &yaw1);
                bno055_quaternion_to_euler(quat2_w, quat2_x, quat2_y, quat2_z, &roll2, &pitch2, &yaw2);
                bno055_read_gyro(address1, &gyro1_x, &gyro1_y, &gyro1_z);
                bno055_read_gyro(address2, &gyro2_x, &gyro2_y, &gyro2_z);
                
                SerialPrintFloat(pitch1);
                SerialPrintStr("   ");
                //SerialPrintInt(accel2_z);
                SerialPrintInt(gyro1_x);
                SerialPrintStr("   ");
                SerialPrintInt(gyro1_y);
                SerialPrintStr("   ");
                SerialPrintInt(gyro1_z);
                //SerialPrintFloat(pitch1);
                SerialPrintStr("\n");
                
                _delay_ms(4);
                }
            //SerialPrintStr("\nend\n");
            }
        }
    return 0;
    }

