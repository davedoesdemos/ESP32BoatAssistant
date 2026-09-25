#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_lv_adapter.h"  // Includes display & input adapters
#include <lvgl.h>

#include "expander.h"
#include "rgblcd43b.h"

extern esp_lcd_touch_handle_t touch_handle;
extern lv_display_t *disp;
extern SemaphoreHandle_t lvgl_mutex;

void touch_reset();
void touch_init();
void display_init();

#endif //DISPLAY_H