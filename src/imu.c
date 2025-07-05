#include "imu.h"



void MPU6500_Init() {
    uint8_t check, data;
    char debug[64];

    // Disable I2C bypass before reading WHO_AM_I to make sure we're talking to the MPU9250

    // Read WHO_AM_I from the MPU9250 to ensure it is connected
    HAL_I2C_Mem_Read(&hi2c1, MPU6500_ADDR, WHO_AM_I	, 1, &check, 1, HAL_MAX_DELAY);
    snprintf(debug, sizeof(debug), "MPU WHO_AM_I: 0x%02X\r\n", check);
    HAL_UART_Transmit(&huart2, (uint8_t*)debug, strlen(debug), HAL_MAX_DELAY);

    // If WHO_AM_I returns 0x71, the MPU9250 is found
    if (check == 0x70) {
        // Wake up the MPU9250 (set power management to 0 to use internal oscillator)
        data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU6500_ADDR, PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);

        // Configure accelerometer: ±2g full scale
        data = 0x00;  // ±2g full scale
        HAL_I2C_Mem_Write(&hi2c1, MPU6500_ADDR, ACCEL_CONFIG, 1, &data, 1, HAL_MAX_DELAY);

        // Configure gyroscope: ±250 degrees per second
        data = 0x00;  // ±250°/s
        HAL_I2C_Mem_Write(&hi2c1, MPU6500_ADDR, GYRO_CONFIG, 1, &data, 1, HAL_MAX_DELAY);

        // Allow some time for the sensor to stabilize
        HAL_Delay(100);

        // Optionally enable I2C bypass if you want to communicate with the magnetometer (AK8963)
        // data = BYPASS_ENABLED;
        // HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, 0x37, 1, &data, 1, HAL_MAX_DELAY);  // INT_PIN_CFG
    } else {
        // Handle MPU9250 error (optional)
        snprintf(debug, sizeof(debug), "MPU6500 not found!\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)debug, strlen(debug), HAL_MAX_DELAY);
    }
}

void MPU6500_ReadAccel(float* ax, float* ay, float* az) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6500_ADDR, ACCEL_XOUT_H,
                    I2C_MEMADD_SIZE_8BIT, data, 6, 100);

    int16_t raw_x = (data[0] << 8) | data[1];
    int16_t raw_y = (data[2] << 8) | data[3];
    int16_t raw_z = (data[4] << 8) | data[5];

    const float scale = 16384.0f; // ±2g scale (16384 LSB/g)
    *ax = raw_x / scale;
    *ay = raw_y / scale;
    *az = raw_z / scale;
}

void MPU6500_ReadGyro(float* gx, float* gy, float* gz) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6500_ADDR, GYRO_XOUT_H,
                    I2C_MEMADD_SIZE_8BIT, data, 6, 100);

    int16_t raw_x = (data[0] << 8) | data[1];
    int16_t raw_y = (data[2] << 8) | data[3];
    int16_t raw_z = (data[4] << 8) | data[5];

    const float scale = 131.0f; // ±250dps scale (131 LSB/dps)
    *gx = raw_x / scale;
    *gy = raw_y / scale;
    *gz = raw_z / scale;
}


