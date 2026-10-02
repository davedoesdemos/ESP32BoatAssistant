#include "nvs_flash.h"
#include <esp_log.h>

//my libraries
#include "i2c.h"
#include "expander.h"
#include "backlight.h"
#include "display.h"
#include "gui.h"
#include "rgblcd43b.h"
#include "wifiscan.h"
#include "wifistation.h"
#include "nmea2k.h"
#include "state.h"
#include "queue_reader.h"
#include "bluetooth.h"

// Logging
static const char *TAG = "Boat Assistant: Main";

void app_main(void)
{
    // Create global message queue
    msg_queue = xQueueCreate(20, sizeof(telemetry_packet_t));

    // Initialise Non-Volatile Storage and reset if there is a problem
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    
    // initialise the rest of the system
    state_init();
    i2c_init();
    expander_init();
    touch_reset();
    touch_init();
    wifi_global_init();
    display_init();
    screen_init(disp);
    init_nmea2000_bus();
    bluetooth_init();
    ESP_LOGI(TAG, "System Inits done");

    // Bluetooth Sender Task
    xTaskCreatePinnedToCore(
        ble_host_task, 
        "ble_host_task", 
        4096, 
        NULL, 
        5, 
        NULL,
        0
    );

    // NMEA Sender Task
    xTaskCreatePinnedToCore(
        nmea_process_to_queue,        // Task function
        "nmea_process_to_queue",      // Task name string
        4096,               // Stack size in bytes
        NULL,               // Parameters passed to the task
        1,                  // Task priority
        NULL,               // Task handle (not needed here)
        0                   // Core ID (0)
    );
    
    // Create backlight timeout task in lvgl
    lv_timer_create(backlight_check_timer_cb, 200, NULL);
    // Global Queue Reader 
    queue_reader_init(msg_queue);
}
