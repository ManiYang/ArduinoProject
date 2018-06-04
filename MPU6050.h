// MPU6050.h
/*
Assume that the AD0 of MPU6050 is connected to GND.
*/

#ifndef MPU6050_H
#define MPU6050_H

#define MPU6050_ADDR 0x68

bool init_MPU6050(bool enable_temperature_sensor);



#endif
