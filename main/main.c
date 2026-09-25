#include "nvs_flash.h"
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
#include "wifiscan.h"
#include "nmea2k.h"

//logging
static const char *TAG = "boat assistant main";

void app_main(void)
{
    lvgl_mutex = xSemaphoreCreateMutex();
    
    if (lvgl_mutex == NULL) {
        // Handle error: out of memory
        return;
    }
    // Initialise Non-Volatile Storage and reset if there is a problem
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    
    // initialise the rest of the system
    i2c_init();
    expander_init();
    touch_reset();
    touch_init();
    wifi_global_init();
    display_init();
    screen_init(disp);
    init_nmea2000_bus();
    
    // Keep app_main alive. Do NOT poll touch coordinates here; 
    // esp_lv_adapter handles it automatically in the background.
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        update_sensor_data((rand() % (45 - 1 + 1)) + 1);
        lv_timer_create(backlight_check_timer_cb, 200, NULL);

        // Spawn NMEA Sender Task on Core 0
        xTaskCreatePinnedToCore(
            nmea_process_to_queue,        // Task function
            "nmea_process_to_queue",      // Task name string
            4096,               // Stack size in bytes
            NULL,               // Parameters passed to the task
            1,                  // Task priority
            NULL,               // Task handle (not needed here)
            0                   // Core ID (0)
        );

        // Spawn NMEA Sender Task on Core 0
        xTaskCreatePinnedToCore(
            nmea_fake_to_queue,        // Task function
            "nmea_fake_to_queue",      // Task name string
            4096,               // Stack size in bytes
            NULL,               // Parameters passed to the task
            1,                  // Task priority
            NULL,               // Task handle (not needed here)
            0                   // Core ID (0)
        );

        // Spawn NMEA Receiver Task on Core 1
        xTaskCreatePinnedToCore(
            update_nmea,        // Task function
            "update_nmea",      // Task name string
            4096,               // Stack size in bytes
            NULL,               // Parameters passed to the task
            1,                  // Task priority
            NULL,               // Task handle (not needed here)
            1                   // Core ID (0)
        );
    }
}
