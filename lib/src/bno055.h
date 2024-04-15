#ifndef BNO055_H
#define BNO055_H

#include <stdint.h>

// Chip identifier
#define BNO055_ID 0xA0

// Register addresses
#define BNO055_CHIP_ID_ADDR 0x00
#define BNO055_SYS_TRIGGER_ADDR 0x3F
#define BNO055_OPR_MODE_ADDR 0x3D
#define BNO055_PWR_MODE_ADDR 0x3E
#define BNO055_ACC_CONFIG_ADDR 0x08
#define BNO055_AXIS_MAP_CONFIG_ADDR 0x41
#define BNO055_AXIS_MAP_SIGN_ADDR 0x42
#define BNO055_PAGE_ID_ADDR 0x07
#define BNO055_QUATERNION_DATA_W_LSB_ADDR 0x20
#define BNO055_UNIT_SEL_ADDR 0x3B
#define BNO055_CALIB_STAT_ADDR 0x35
// Additional sensor data registers as needed
#define BNO055_ACCEL_DATA_X_LSB_ADDR 0x08
#define BNO055_ACCEL_DATA_Z_LSB_ADDR 0x0c
#define bno055_LIA_Data_X_LSB_ADDR 0x28
#define BNO055_GYRO_DATA_X_LSB_ADDR 0x14


// Operational mode register values
#define CONFIG_MODE 0x00
#define POWER_MODE_NORMAL 0x00

#define ACCONLY 0X01
#define MAGONLY 0X02
#define GYRONLY 0X03
#define ACCMAG 0X04
#define ACCGYRO 0X05
#define MAGGYRO 0X06
#define AMG 0X07
#define IMUPLUS 0X08
#define COMPASS 0X09
#define M4G 0X0A
#define NDOF_FMC_OFF 0X0B
#define NDOF 0X0C

// register values for accelerometer range configuration
#define BNO055_ACC_RANGE_2G 0
#define BNO055_ACC_RANGE_4G 1
#define BNO055_ACC_RANGE_8G 2
#define BNO055_ACC_RANGE_16G 3

// Function prototypes
void bno055_init(uint8_t address);
void bno055_set_opperation_mode(uint8_t address, uint8_t mode);
void bno055_read_quaternion(uint8_t address, float *w, float *x, float *y, float *z);
void bno055_read_accelerometer(uint8_t address, int16_t *x, int16_t *y, int16_t *z);
void bno055_read_gyro(uint8_t address, int16_t *x, int16_t *y, int16_t *z); 
void bno055_set_power_mode(uint8_t address, uint8_t mode);
void bno055_set_unit_sel(uint8_t address, uint8_t unit_sel);
void bno055_config_accelerometer(uint8_t address, uint8_t regAddr, uint8_t mask, uint8_t value);
void bno055_config_accelerometer_g_range(uint8_t address, uint8_t g_range_setting);
void change_page(uint8_t address, uint8_t page);
void check_write_success(uint8_t address, uint8_t regAddr, uint8_t value);
void check_id(uint8_t address);
void bno055_read_accelerometer_z(uint8_t address, int16_t *z);
void bno055_quaternion_to_euler(float w, float x, float y, float z, float *roll, float *pitch, float *yaw);
void bno055_get_calibration(uint8_t address, uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag);
void bno055_calibrate(uint8_t address);

#endif // BNO055_H
