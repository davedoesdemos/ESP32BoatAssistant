#ifndef EXPANDER_H
#define EXPANDER_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"

// CH422G Expander Constants
#define CH422G_I2C_ADDR             0x24    // Base Expander Address
#define CH422G_I2C_ADDR2             0x38    // Second Expander Address
#define CH422G_REG_IN               0x26    // Target Input Byte Register

//CH422 Expander pin masks for Waveshare 4.3b
#define TOUCH_RESET_PIN_MASK (1 << 1)
#define BACKLIGHT_PIN_MASK (1 << 2)
#define DISPLAY_RESET_PIN_MASK (1<<3)

extern uint8_t expander_pins;
    
extern i2c_master_dev_handle_t expander_dev_handle;
extern i2c_master_dev_handle_t expander_dev_handle2;

esp_err_t expander_output_init(i2c_master_dev_handle_t expander_handle);
esp_err_t expander_set_pins(i2c_master_dev_handle_t expander_handle2, uint8_t expander_pins);
void expander_init();

#endif //EXPANDER_H