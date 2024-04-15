#include "bno055.h"
#include "twi.h" 
#include <avr/io.h>
#include "serial.h"
#include "delay.h"
#include <math.h> // Include the math.h header file
#define M_PI 3.14159265358979323846 // Define the value of M_PI constant

uint8_t error = 0;

void bno055_init(uint8_t address) {
    TWI_init();
    twi_write_byte(address, BNO055_SYS_TRIGGER_ADDR, 0x20); // Soft reset the sensor
    Delay_ms(650); // Wait for the reset to take effect

    //check_id(address);
    bno055_config_accelerometer_g_range(address, BNO055_ACC_RANGE_16G);
    bno055_set_opperation_mode(address, NDOF);
    //bno055_calibrate(address);

    if (error == 1) {
        SerialPrintStr("Error initializing BNO055 sensor\n");
    }
    else {
        SerialPrintStr("BNO055 sensor initialized at address: ");
        SerialPrintByte_as_hex(address);
        SerialPrintStr("\n");
    }
}

void bno055_set_opperation_mode(uint8_t address, uint8_t mode) {
    twi_write_byte(address, BNO055_OPR_MODE_ADDR, CONFIG_MODE);
    Delay_ms(25);
    twi_write_byte(address, BNO055_OPR_MODE_ADDR, mode);
    Delay_ms(30);
    check_write_success(address, BNO055_OPR_MODE_ADDR, mode); 
}

void bno055_set_power_mode(uint8_t address, uint8_t mode) {
    twi_write_byte(address, BNO055_OPR_MODE_ADDR, CONFIG_MODE);
    Delay_ms(30);
    twi_write_byte(address, BNO055_PWR_MODE_ADDR, mode);
    Delay_ms(30);
    check_write_success(address, BNO055_PWR_MODE_ADDR, mode);
}

void bno055_read_accelerometer(uint8_t address, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];
    // change bno055_LIA_Data_X_LSB_ADDR to BNO055_ACCEL_DATA_X_LSB_ADDR for g-range
    twi_read_buffer(address, bno055_LIA_Data_X_LSB_ADDR, buffer, 6);
    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);
}

void bno055_read_gyro(uint8_t address, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];
    twi_read_buffer(address, BNO055_GYRO_DATA_X_LSB_ADDR, buffer, 6);
    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);
}

void bno055_read_quaternion(uint8_t address, float *w, float *x, float *y, float *z) {
    uint8_t buffer[8];
    twi_read_buffer(address, BNO055_QUATERNION_DATA_W_LSB_ADDR, buffer, 8);
    *w = (float)((int16_t)((buffer[1] << 8) | buffer[0])) / 16384.0;
    *x = (float)((int16_t)((buffer[3] << 8) | buffer[2])) / 16384.0;
    *y = (float)((int16_t)((buffer[5] << 8) | buffer[4])) / 16384.0;
    *z = (float)((int16_t)((buffer[7] << 8) | buffer[6])) / 16384.0;
}

void bno055_quaternion_to_euler(float w, float x, float y, float z, float *roll, float *pitch, float *yaw) {

    *roll = atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y));
    *pitch = asin(2.0 * (w * y - z * x));
    *yaw = atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z));
    *roll = *roll * 180.0 / M_PI;
    *pitch = *pitch * 180.0 / M_PI;
    *yaw = *yaw * 180.0 / M_PI;
}

void bno055_read_accelerometer_z(uint8_t address, int16_t *z) {
    uint8_t buffer[2];
    twi_read_buffer(address, BNO055_ACCEL_DATA_Z_LSB_ADDR, buffer, 2);
    *z = (int16_t)((buffer[1] << 8) | buffer[0]);
}

void bno055_get_calibration(uint8_t address, uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag) {
    uint8_t calData = twi_read_byte(address, BNO055_CALIB_STAT_ADDR);
    *sys = (calData >> 6) & 0x03;
    *gyro = (calData >> 4) & 0x03;
    *accel = (calData >> 2) & 0x03;
    *mag = calData & 0x03;
    // print calibration data
    SerialPrintStr("Sys: ");
    SerialPrintByte_as_binary(*sys);
    SerialPrintStr(" Gyro: ");
    SerialPrintByte_as_binary(*gyro);
    SerialPrintStr(" Accel: ");
    SerialPrintByte_as_binary(*accel);
    SerialPrintStr(" Mag: ");
    SerialPrintByte_as_binary(*mag);
    SerialPrintStr("\n");
}

void bno055_calibrate(uint8_t address) {
    uint8_t sys = 0, gyro = 0, accel = 0, mag = 0;
    while (sys != 3 || gyro != 3 || accel != 3 || mag != 3) {
        bno055_get_calibration(address, &sys, &gyro, &accel, &mag);
        Delay_ms(10);
    }
}


void change_page(uint8_t address, uint8_t page) {
    twi_write_byte(address, BNO055_PAGE_ID_ADDR, page);
    Delay_ms(10);
}

void bno055_config_accelerometer_g_range(uint8_t address,uint8_t g_range_setting) {

    change_page(address, 1);  // Set the page to 1 to access ACC_CONFIG register
    Delay_ms(10);   // Wait for the page to change

    const uint8_t mask = 0x03;  // mask 0000 0011
    uint8_t regValue = twi_read_byte(address, BNO055_ACC_CONFIG_ADDR);  // Read the current contents of the register
    regValue = ((regValue & ~mask) | (g_range_setting & mask)); // Clear the bits in the mask and set the new value
    twi_write_byte(address, BNO055_ACC_CONFIG_ADDR, regValue); // Write the modified value back to the register
    Delay_ms(50); // Wait for the configuration to take effect

    check_write_success(address, BNO055_ACC_CONFIG_ADDR, regValue);

    change_page(address, 0); // Set the page back to 0
    Delay_ms(10);
}

void bno055_set_unit_sel(uint8_t address, uint8_t unit_sel) {
    uint8_t mask = 0x01; // mask the unit_sel value 0000 0001
    uint8_t regValue = twi_read_byte(address, BNO055_UNIT_SEL_ADDR);  // Read the current contents of the register   
    regValue = ((regValue & ~mask) | (unit_sel & mask)); // Clear the bits in the mask and set the new value
    twi_write_byte(address, BNO055_UNIT_SEL_ADDR, regValue); // Write the modified value back to the register
    Delay_ms(10);  // Wait for the configuration to take effect
}

void check_id(uint8_t address) {
    uint8_t id = twi_read_byte(address, BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) {
        SerialPrintStr("id not set, id = ");
        SerialPrintByte_as_binary(id);
        SerialPrintStr("\n");
        return;
    }
}

void bno055_config_accelerometer(uint8_t address, uint8_t regAddr, uint8_t mask, uint8_t value) {
    uint8_t regValue = twi_read_byte(address, regAddr);
    regValue &= ~mask;
    regValue |= (value & mask);
    twi_write_byte(address, regAddr, regValue);
    Delay_ms(10); // Wait for the configuration to take effect
}


void check_write_success(uint8_t address, uint8_t regAddr, uint8_t value){
    uint8_t status = twi_read_byte(address, regAddr);
    if (status != value) {
        SerialPrintStr("\nWrite not successful to register address: ");
        SerialPrintByte_as_hex(regAddr);
        SerialPrintStr("\nCurrent register value: ");
        SerialPrintByte_as_binary(status);
        SerialPrintStr("\n\n");
        error = 1;

    } 

}
