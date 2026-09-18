#ifndef EXPANDER_H
#define EXPANDER_H

#include <esp_err.h>
#include <driver/i2c_master.h>

// CH422G Expander Constants
#define CH422G_I2C_ADDR             0x24    // Base Expander Address
#define CH422G_I2C_ADDR2             0x38    // Second Expander Address
#define CH422G_REG_IN               0x26    // Target Input Byte Register

esp_err_t expander_output_init(i2c_master_dev_handle_t expander_handle);
esp_err_t expander_set_pins(i2c_master_dev_handle_t expander_handle2, uint8_t expander_pins);

#endif //EXPANDER_H