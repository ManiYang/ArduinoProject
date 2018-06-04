// MPU6050.cpp

#include <Wire.h>
#include "MPU6050.h"
#include "I2C_my_api.h"

bool init_MPU6050(bool enable_temperature_sensor) 
//Wire (I2C) must be already started.
{
  // exit sleep mode
  if(!I2C_write_byte(MPU6050_ADDR, 0x6B, 
                     enable_temperature_sensor ? 0x00 : 0x08))
    return false;

  // set sampling rate divider to 7 (=> 1kHz)
  if(!I2C_write_byte(MPU6050_ADDR, 0x19, 0x07))
    return false;
  
  //
  return true;
}
