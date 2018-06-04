// MPU6050.cpp

#include <Wire.h>
#include "MPU6050.h"

bool init_MPU6050(bool enable_temperature_sensor) 
//Wire (I2C) must be already started.
{
  // exit sleep mode
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); //register to write
  Wire.write(enable_temperature_sensor ? 0x00 : 0x08);
  if(Wire.endTransmission() != 0) 
    return false;

  // set sampling rate divider to 7 (=> 1kHz)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x19); //register to write
  Wire.write(0x07);
  if(Wire.endTransmission() != 0) 
    return false;
  
  return true;
}
