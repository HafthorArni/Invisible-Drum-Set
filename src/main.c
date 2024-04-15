#ifndef F_CPU
#define F_CPU 16000000UL // Standard Arduino clock speed
#endif

#include <stdbool.h>
#include "serial.h"
#include "bno055.h"
#include "timer.h"
#include "digital_in.h"
#include "analog_in.h"
#include "delay.h"
#include <math.h>

#define address1 0x28 // I2C address of the BNO055 sensor
#define address2 0x29 // I2C address of the BNO055 sensor

// Variables for debouncing first button
#define DEBOUNCE_DELAY1 50 // Debounce delay in milliseconds
unsigned long lastDebounceTime1 = 0; // the last time the output pin was toggled
uint8_t buttonprinted1 = false; // Initialize to false
#define button1 6 // first button pin

// Variables for debouncing second button
#define DEBOUNCE_DELAY2 50 // Debounce delay in milliseconds
unsigned long lastDebounceTime2 = 0; // the last time the output pin was toggled
uint8_t buttonprinted2 = false; // Initialize to false
#define button2 7 // second button pin

// Variables for debouncing the piezo
#define DEBOUNCE_DELAY3 100 // Debounce delay in milliseconds
unsigned long lastDebounceTime3 = 0; // the last time the output pin was toggled
uint8_t piezoprinted3 = false; // Initialize to false
#define piezo 0 // piezo pin

// Variables for yaw offset
float yawOffset1 = 0, yawOffset2 = 0;

// Function to enable internal pullups on SDA and SCL lines
void enableInternalPullups(void) {
    // Set SDA and SCL as input
    DDRC &= ~((1 << DDC4) | (1 << DDC5));
    // Enable pull-up resistors on SDA and SCL lines
    PORTC |= (1 << PORTC4) | (1 << PORTC5);
}

// Function to normalize yaw values to the 0 - 360 range
float normalizeYaw(float yaw) {
    yaw = fmod(yaw, 360.0); // Use floating point modulo to wrap yaw
    if (yaw < 0) {
        yaw += 360.0; // Ensure yaw is positive
    }
    return yaw;
}


int main(void) {
    enableInternalPullups(); // Enable internal pullups
    SerialBegin(115200); // Initialize serial communication
    init_millis(F_CPU);  // Initialize millis using the custom time library
    digital_in_init(button1);  // Initialize digital input on pin 6
    digital_in_init(button2);  // Initialize digital input on pin 7
    adc_init(piezo);  // Initialize analog input on pin A0
    bno055_init(address1);       // Initialize the first BNO055 sensor
    bno055_init(address2);       // Initialize the second BNO055 sensor

    // quaternion data variables
    float quat1_w, quat1_x, quat1_y, quat1_z; // Quaternion data variables
    float quat2_w, quat2_x, quat2_y, quat2_z; // Quaternion data variables
    // euler data variables
    float roll1, pitch1, yaw1; // Euler data variables
    float roll2, pitch2, yaw2; // Euler data variables
    // gyro data variables
    int16_t gyro1_x, gyro1_y, gyro1_z; // Gyro data variables
    int16_t gyro2_x, gyro2_y, gyro2_z; // Gyro data variables

    bool printedForGyro1y = false; // Flag to prevent multiple prints for gyro1_y
    bool printedForGyro2y = false; // Flag to prevent multiple prints for gyro2_y

    uint16_t piezoValue; // Initialize piezo value

    while (1) {
        bno055_read_quaternion(address1, &quat1_w, &quat1_x, &quat1_y, &quat1_z);
        bno055_read_quaternion(address2, &quat2_w, &quat2_x, &quat2_y, &quat2_z);
        bno055_quaternion_to_euler(quat1_w, quat1_x, quat1_y, quat1_z, &roll1, &pitch1, &yaw1);
        bno055_quaternion_to_euler(quat2_w, quat2_x, quat2_y, quat2_z, &roll2, &pitch2, &yaw2);
        bno055_read_gyro(address1, &gyro1_x, &gyro1_y, &gyro1_z);
        bno055_read_gyro(address2, &gyro2_x, &gyro2_y, &gyro2_z);

        yaw1 = normalizeYaw(yaw1 - yawOffset1); // Adjust yaw1 by its offset and normalize
        yaw2 = normalizeYaw(yaw2 - yawOffset2); // Adjust yaw2 by its offset and normalize


        // right hand logic
        // if yaw in the range of 20-120 then play snare drum
        if (yaw1 >= 20 && yaw1 <= 120) {
            if (gyro1_y < -2500 && !printedForGyro1y) {
                SerialPrintStr("0"); // snare drum
                printedForGyro1y = true; // Mark as printed
            } else if (gyro1_y >= -2500 && printedForGyro1y) {
                printedForGyro1y = false;
            }
        }
        // if yaw in the range of 340 - 20 then play high tom
        else if (yaw1 >= 340 || yaw1 <= 20) {
            if (gyro1_y < -2500 && !printedForGyro1y) {
                // if pitch over 55 degrees play crash cymbal
                if (pitch1 > 50) {
                    SerialPrintStr("5"); // crash cymbal
                }
                else {
                    SerialPrintStr("3"); // high tom
                }
                printedForGyro1y = true; // Mark as printed
            } else if (gyro1_y >= -2500 && printedForGyro1y) {
                printedForGyro1y = false;
            }
        }

        // if yaw in the range of 305-340 then play mid tom
        else if (yaw1 >= 305 && yaw1 <= 340) {
            if (gyro1_y < -2500 && !printedForGyro1y) {
                // if pitch over 55 degrees play ride cymbal
                if (pitch1 > 50) {
                    SerialPrintStr("6"); // ride cymbal
                }
                else {
                    SerialPrintStr("4"); // mid tom
                }
                printedForGyro1y = true; // Mark as printed
            } else if (gyro1_y >= -2500 && printedForGyro1y) {
                printedForGyro1y = false;
            }
        }
        // if yaw in the range of 200-305 then play floor tom
        else if (yaw1 >= 200 && yaw1 <= 305) {
            if (gyro1_y < -2500 && !printedForGyro1y) {
                if (pitch1 > 30) {
                    SerialPrintStr("6"); // ride cymbal 
                }
                else {
                SerialPrintStr("7"); // floor tom
                }
                printedForGyro1y = true; // Mark as printed
            } else if (gyro1_y >= -2500 && printedForGyro1y) {
                printedForGyro1y = false;
            }
        }

        // left hand logic
        // if yaw in the range of 350-60 then play snare drum or hi-hat
        if (yaw2 >= 350 || yaw2 <= 100) {
            if (gyro2_y < -2500 && !printedForGyro2y) {
                // if facing up and not rotating fast around z axis
                if (pitch2 > 30 && gyro2_z > -2000) {
                    SerialPrintStr("1"); // hi-hat
                }
                else {
                    SerialPrintStr("0"); // snare drum
                }
                printedForGyro2y = true; // Mark as printed
            } else if (gyro2_y >= -2500 && printedForGyro2y) {
                printedForGyro2y = false;
            }
        }
        // if yaw in the range of 325-350 then play high tom or crash cymbal
        else if (yaw2 >= 325 && yaw2 <= 350) {
            if (gyro2_y < -2500 && !printedForGyro2y) {
                // if pitch over 55 degrees play crash cymbal
                if (pitch2 > 50) {
                    SerialPrintStr("5"); // crash cymbal
                }
                else {
                    SerialPrintStr("3"); // high tom
                }
                printedForGyro2y = true; // Mark as printed
            } else if (gyro2_y >= -2500 && printedForGyro2y) {
                printedForGyro2y = false;
            }
        }
        // if yaw in the range of 300-325 then play mid tom or ride cymbal
        else if (yaw2 >= 300 && yaw2 <= 325) {
            if (gyro2_y < -2500 && !printedForGyro2y) {
                // if pitch over 55 degrees play ride cymbal
                if (pitch2 > 50) {
                    SerialPrintStr("6"); // ride cymbal
                }
                else {
                    SerialPrintStr("4"); // mid tom
                }
                printedForGyro2y = true; // Mark as printed
            } else if (gyro2_y >= -2500 && printedForGyro2y) {
                printedForGyro2y = false;
            }
        }
        // if yaw in the range of 200-300 then play floor tom
        else if (yaw2 >= 200 && yaw2 <= 300) {
            if (gyro2_y < -2500 && !printedForGyro2y) {
                if (pitch2 > 30) {
                    SerialPrintStr("6"); // ride cymbal 
                }
                else {
                SerialPrintStr("7"); // floor tom
                }
                printedForGyro2y = true; // Mark as printed
            } else if (gyro2_y >= -2500 && printedForGyro2y) {
                printedForGyro2y = false;
            }
        }


        // right hand button logic
        if (millis() - lastDebounceTime1 > DEBOUNCE_DELAY1) {
            uint8_t reading1 = digital_in_read(button1);  // Read the right hand button 

            if (reading1 == 1 && !buttonprinted1) {
                buttonprinted1 = true;
                SerialPrintStr("2"); // kick drum
                }
            if (reading1 == 0) {
                buttonprinted1 = false;
                }
            lastDebounceTime1 = millis();
            }

        // left hand button logic
        if (millis() - lastDebounceTime2 > DEBOUNCE_DELAY2) {
            uint8_t reading2 = digital_in_read(button2);  // Read the left hand button

            if (reading2 == 1 && !buttonprinted2) {
                // Read current yaw values to set them as new zero (north)
                bno055_read_quaternion(address1, &quat1_w, &quat1_x, &quat1_y, &quat1_z);
                bno055_quaternion_to_euler(quat1_w, quat1_x, quat1_y, quat1_z, &roll1, &pitch1, &yaw1);
                bno055_read_quaternion(address2, &quat2_w, &quat2_x, &quat2_y, &quat2_z);
                bno055_quaternion_to_euler(quat2_w, quat2_x, quat2_y, quat2_z, &roll2, &pitch2, &yaw2);
                // reset the yaw values
                yawOffset1 = yaw1;
                yawOffset2 = yaw2;
                //SerialPrintStr("button 2 pressed\n");
                buttonprinted2 = true;
                }
            if (reading2 == 0) {
                buttonprinted2 = false;
                }
            lastDebounceTime2 = millis();
            }

        // piezo logic
        if (millis() - lastDebounceTime3 > DEBOUNCE_DELAY3) {
            piezoValue = adc_read(piezo); // Read the piezo value
            if (piezoValue > 50 && !piezoprinted3) {
                SerialPrintStr("2"); // kick drum
                piezoprinted3 = true;
            }
            if (piezoValue <= 50) {
                piezoprinted3 = false;
            }
            lastDebounceTime3 = millis();
        }

        //SerialPrintFloat(yaw1);
        //SerialPrintStr("   ");
        //SerialPrintFloat(yaw2);
        //SerialPrintStr("\n");
    
    }
}