#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <stdio.h>
#include <driver/i2c_master.h>
#define BACKLIGHT_PIN_MASK (1 << 2)

uint8_t backlight_on(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins);
uint8_t backlight_off(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins);

#endif //BACKLIGHT_H