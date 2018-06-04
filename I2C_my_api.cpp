#include <Arduino.h>
#include <Wire.h>
#include "I2C_my_api.h"

bool I2C_write_byte(const short device_addr, const short reg, const short data)
//Wire (I2C) must be already started.
{
  Wire.beginTransmission(device_addr);
  Wire.write(reg); 
  Wire.write(data);
  return (Wire.endTransmission() == 0);
}

bool I2C_read_bytes(const short device_addr, const short reg, const int bytes_to_read, char *buffer)
//Wire (I2C) must be already started.
{
  Wire.beginTransmission(device_addr);
  Wire.write(reg); 
  if(Wire.endTransmission() != 0)
    return false;
  if(Wire.requestFrom(device_addr, bytes_to_read) != bytes_to_read) 
    return false;
  for(int i=0; i<bytes_to_read; i++)
    buffer[i] = Wire.read();
  
  return true;
}
