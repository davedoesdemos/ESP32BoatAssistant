#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <stdio.h>
#include <driver/i2c_master.h>
#include "expander.h"

#define BACKLIGHT_PIN_MASK (1 << 2)

extern uint32_t BACKLIGHT_TIMEOUT_MS; //default backlight timeout 10 seconds

uint8_t backlight_on(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins);
uint8_t backlight_off(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins);
uint8_t set_backlight_state(bool turn_on, uint8_t expander_pins, i2c_master_dev_handle_t expander_handle);

#endif //BACKLIGHT_H