#ifndef GUI_H
#define GUI_H

#include <lvgl.h>
#include "esp_lv_adapter.h"  // Includes display & input adapters

#include "backlight.h"
#include "expander.h"
#include "screens/screen_settings.h"
#include "screens/screen_environment.h"
#include "screens/screen_wifi.h"
#include "screens/screen_1.h"
#include "screens/screen_2.h"

void set_status(const char * status);
void backlight_check_timer_cb(lv_timer_t * timer);
void screen_init(lv_display_t *disp);

#endif // GUI_H