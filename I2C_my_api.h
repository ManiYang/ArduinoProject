#ifndef I2C_MY_API_H
#define I2C_MY_API_H

bool I2C_write_byte(const short device_addr, const short reg, const short data);
//Wire (I2C) must be already started.

bool I2C_read_bytes(const short device_addr, const short reg, const int bytes_to_read, char *buffer);
//Wire (I2C) must be already started.

#endif
