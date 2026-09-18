#include <stdio.h>
#include "expander.h"
#include "i2c.h"

i2c_master_dev_handle_t expander_dev_handle = NULL;
i2c_master_dev_handle_t expander_dev_handle2 = NULL;

    //Set up pins on expander code replaced with 0x0E below
    //expander_pins = 0x00;
    //expander_pins |= BACKLIGHT_PIN_MASK;
    //expander_pins |= DISPLAY_RESET_PIN_MASK;
    //expander_pins |= TOUCH_RESET_PIN_MASK;
uint8_t expander_pins = 0x0E;

esp_err_t expander_output_init(i2c_master_dev_handle_t expander_handle)
{
    esp_err_t ret;
    uint8_t write_buf;

    // Configure CH422G to output mode
    write_buf = 0x01;
    ret = i2c_master_transmit(expander_handle, &write_buf, 1, I2C_MASTER_TIMEOUT_MS);
    if (ret != ESP_OK) return ret;

    return ret;
}
esp_err_t expander_set_pins(i2c_master_dev_handle_t expander_handle2, uint8_t expander_pins)
{
    esp_err_t ret;
    // push expander pin config
    ret = i2c_master_transmit(expander_handle2, &expander_pins, 1, I2C_MASTER_TIMEOUT_MS);
    //printf("OFFPull the backlight pin high to light the screen backlight\n");
    return ret;
}
