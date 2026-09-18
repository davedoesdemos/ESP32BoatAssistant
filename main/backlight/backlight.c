#include "backlight.h"
//logging
static const char *TAG = "boat assistant backlight";

uint32_t BACKLIGHT_TIMEOUT_MS = 10000; //default backlight timeout 10 seconds

uint8_t backlight_on(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins){
    expander_pins |= BACKLIGHT_PIN_MASK;
    expander_set_pins(expander_handle, expander_pins);
    return expander_pins;
}

uint8_t backlight_off(i2c_master_dev_handle_t expander_handle, uint8_t expander_pins){
    expander_pins &= ~BACKLIGHT_PIN_MASK;
    expander_set_pins(expander_handle, expander_pins);
    return expander_pins;
}

uint8_t set_backlight_state(bool turn_on, uint8_t expander_pins, i2c_master_dev_handle_t expander_handle) {
    if (turn_on) {
        expander_pins |= BACKLIGHT_PIN_MASK;
        expander_set_pins(expander_handle, expander_pins);
        return expander_pins;
    } else {
        expander_pins &= ~BACKLIGHT_PIN_MASK;
        expander_set_pins(expander_handle, expander_pins);
        return expander_pins;
    }
}