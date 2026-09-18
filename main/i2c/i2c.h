#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>

// Master I2C Settings
#define I2C_MASTER_SDA_IO           8       // Hardware Pin SDA
#define I2C_MASTER_SCL_IO           9       // Hardware Pin SCL
#define I2C_MASTER_PORT             I2C_NUM_0
#define I2C_MASTER_TIMEOUT_MS       1000

extern i2c_master_bus_handle_t global_bus_handle;

esp_err_t init_i2c_bus(i2c_master_bus_handle_t *out_bus_handle);
esp_err_t init_i2c_device(int device_address, i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_dev_handle);
void i2c_init();

#endif // I2C_H