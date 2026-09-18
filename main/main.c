#include <stdio.h>
#include <lvgl.h>
#include <esp_err.h>
#include <esp_log.h>
#include "esp_check.h"        // Dependent header file
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_gt911.h"  // Example with GT911
#include "driver/i2c_master.h"
#include "esp_lv_adapter.h"  // Includes display & input adapters
//my libraries
#include "i2c.h"
#include "expander.h"
#include "backlight.h"
#include "gui.h"
#include "rgblcd43b.h"

uint8_t expander_pins = 0x00;

//logging
static const char *TAG = "boat assistant";

//CH422 Expander pin masks for Waveshare 4.3b
#define TOUCH_RESET_PIN_MASK (1 << 1)
#define BACKLIGHT_PIN_MASK (1 << 2)
#define DISPLAY_RESET_PIN_MASK (1<<3)

//static lv_obj_t * slider_label;

typedef struct {
    i2c_master_bus_handle_t global_bus_handle;
    i2c_master_dev_handle_t expander_dev_handle;
    i2c_master_dev_handle_t expander_dev_handle2;
}  i2c_config;

i2c_config i2c_settings = {NULL, NULL, NULL};

// Timer callback function checked periodically by LVGL
static void backlight_check_timer_cb(lv_timer_t * timer) {
    static bool backlight_is_on = true;
    
    // Get inactive time from the default display
    // Note: For LVGL v8, use: uint32_t idle_time = lv_disp_get_inactive_time(NULL);
    uint32_t idle_time = lv_display_get_inactive_time(lv_display_get_default());

    if (idle_time >= BACKLIGHT_TIMEOUT_MS) {
        if (backlight_is_on) {
            printf("backlight on");
            expander_pins = set_backlight_state(false, expander_pins, i2c_settings.expander_dev_handle2); // Turn off backlight
            backlight_is_on = false;
        }
    } else {
        if (!backlight_is_on) {
            printf("backlight off");
            expander_pins = set_backlight_state(true, expander_pins, i2c_settings.expander_dev_handle2);  // Turn back on if there is user activity
            backlight_is_on = true;
        }
    }
}


void app_main(void)
{
    //Set up pins on expander
    expander_pins |= BACKLIGHT_PIN_MASK;
    expander_pins |= DISPLAY_RESET_PIN_MASK;
    expander_pins |= TOUCH_RESET_PIN_MASK;
    //i2c_config i2c_settings = {NULL, NULL, NULL};

    //Initialise the i2c bus
    if (init_i2c_bus(&i2c_settings.global_bus_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C Master Bus!");
        return;
    }
    ESP_LOGI(TAG, "I2C Master Bus initialized successfully.");

    //initialise expander hardware1
    if (init_i2c_device(CH422G_I2C_ADDR ,i2c_settings.global_bus_handle, &i2c_settings.expander_dev_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to attach Expander 1");
        return;
    }
    expander_output_init(i2c_settings.expander_dev_handle);
    ESP_LOGI(TAG, "Expander 1 (0x%02X) registered.", CH422G_I2C_ADDR);

    //initialise expander hardware2
    if (init_i2c_device(CH422G_I2C_ADDR2 ,i2c_settings.global_bus_handle, &i2c_settings.expander_dev_handle2) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to attach I2C peripheral device!");
        return;
    }
    ESP_LOGI(TAG, "Expander 2 (0x%02X) registered.", CH422G_I2C_ADDR2);

    // Hardware reset the GT911 before init
    // Pull Reset Low, wait, pull High
    expander_pins &= ~TOUCH_RESET_PIN_MASK; 
    expander_set_pins(i2c_settings.expander_dev_handle2, expander_pins);
    vTaskDelay(pdMS_TO_TICKS(20)); 
    expander_pins |= TOUCH_RESET_PIN_MASK;
    expander_set_pins(i2c_settings.expander_dev_handle2, expander_pins);
    vTaskDelay(pdMS_TO_TICKS(100)); // Give GT911 time to boot up

    // Initialize touch IO (I2C)
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_settings.global_bus_handle, &io_config, &io_handle));

    // Configure touch panel
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = RGB_LCD_H_RES,          // Screen width
        .y_max = RGB_LCD_V_RES,          // Screen height
        .rst_gpio_num = GPIO_NUM_NC,  // Reset pin (or GPIO_NUM_NC)
        .int_gpio_num = GPIO_NUM_3,   // Interrupt pin (or GPIO_NUM_NC)
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };

    // Initialize touch controller
    esp_lcd_touch_handle_t touch_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(io_handle, &tp_cfg, &touch_handle));

    const esp_lv_adapter_rotation_t rotation = ESP_LV_ADAPTER_ROTATE_0;
    const esp_lv_adapter_tear_avoid_mode_t tear_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_DEFAULT_RGB;
    const uint8_t frame_buffer_count = esp_lv_adapter_get_required_frame_buffer_count(tear_mode, rotation);

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_io_handle_t panel_io = NULL;
    // Call library function to handle allocation and setup
    ESP_ERROR_CHECK(rgblcd_panel_init(frame_buffer_count, &panel_handle));

    //https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/tools/esp_lvgl_adapter.html
    // Step 1: Initialize the adapter
    esp_lv_adapter_config_t cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(esp_lv_adapter_init(&cfg));
    // Register the display (choose macro by interface)
    esp_lv_adapter_display_config_t disp_cfg = ESP_LV_ADAPTER_DISPLAY_RGB_DEFAULT_CONFIG(
        panel_handle,    // LCD panel handle
        panel_io,        // LCD panel IO handle (can be NULL for some interfaces)
        RGB_LCD_H_RES,   // Horizontal resolution
        RGB_LCD_V_RES,   // Vertical resolution
        rotation         // Rotation
    );
    disp_cfg.profile.use_psram = true;

    lv_display_t *disp = esp_lv_adapter_register_display(&disp_cfg);
    assert(disp != NULL);
    // Register input device
    // Create touch handle using esp_lcd_touch API (implementation omitted here)
    // esp_lcd_touch_handle_t touch_handle = /* ... */;
    esp_lv_adapter_touch_config_t touch_cfg = ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(disp, touch_handle);
    lv_indev_t *touch = esp_lv_adapter_register_touch(&touch_cfg);
    assert(touch != NULL);



    screen_init(disp);
    
    // Keep app_main alive. Do NOT poll touch coordinates here; 
    // esp_lv_adapter handles it automatically in the background.
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        update_sensor_data((rand() % (45 - 1 + 1)) + 1);
        lv_timer_create(backlight_check_timer_cb, 200, NULL);
        //expander_pins = backlight_on(i2c_settings.expander_dev_handle2, expander_pins);
        //vTaskDelay(pdMS_TO_TICKS(1000));
        //expander_pins = backlight_off(i2c_settings.expander_dev_handle2, expander_pins);
    }
}
