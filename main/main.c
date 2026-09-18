//#include <stdio.h>
//#include <lvgl.h>
//#include <esp_err.h>
//#include <esp_log.h>
//#include "esp_check.h"        // Dependent header file
//#include "esp_lcd_panel_ops.h"
//#include "esp_lcd_panel_rgb.h"
//#include "esp_lcd_touch.h"
//#include "esp_lcd_touch_gt911.h"  // Example with GT911
//#include "driver/i2c_master.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "esp_lv_adapter.h"  // Includes display & input adapters

//my libraries
#include "i2c.h"
#include "expander.h"
#include "backlight.h"
#include "display.h"
#include "gui.h"
#include "rgblcd43b.h"

//logging
static const char *TAG = "boat assistant main";

void app_main(void)
{
    i2c_init();
    expander_init();
    touch_reset();
    touch_init();
    display_init();
    screen_init(disp);
    
    // Keep app_main alive. Do NOT poll touch coordinates here; 
    // esp_lv_adapter handles it automatically in the background.
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        update_sensor_data((rand() % (45 - 1 + 1)) + 1);
        lv_timer_create(backlight_check_timer_cb, 200, NULL);
        //expander_pins = backlight_on(expander_dev_handle2, expander_pins);
        //vTaskDelay(pdMS_TO_TICKS(1000));
        //expander_pins = backlight_off(expander_dev_handle2, expander_pins);
    }
}
