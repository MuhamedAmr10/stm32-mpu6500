#ifndef IMU_H
#define IMU_H

#include "main.h"
#include <stdio.h>
#include <string.h>

// I2C and UART handlers
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

// MPU9250 I2C address
#define MPU6500_ADDR  (0x68 << 1)

// Registers
#define WHO_AM_I 0x75  // MPU6500 returns 0x75 (MPU9250 returns 0x71)
#define PWR_MGMT_1    0x6B
#define ACCEL_XOUT_H  0x3B
#define ACCEL_CONFIG  0x1C
#define GYRO_XOUT_H   0x43
#define GYRO_CONFIG   0x1B



// Function declarations
void MPU6500_Init();
void MPU6500_ReadAccel(float* ax, float* ay, float* az);
void MPU6500_ReadGyro(float* gx, float* gy, float* gz);

#endif
