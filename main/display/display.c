#include "display.h"

esp_lcd_touch_handle_t touch_handle = NULL;
lv_display_t *disp = NULL;
// Define the variable
SemaphoreHandle_t lvgl_mutex = NULL;

void touch_reset(){
    // Hardware reset the GT911 before init
    // Pull Reset Low, wait, pull High
    expander_pins &= ~TOUCH_RESET_PIN_MASK; 
    expander_set_pins(expander_dev_handle2, expander_pins);
    vTaskDelay(pdMS_TO_TICKS(20)); 
    expander_pins |= TOUCH_RESET_PIN_MASK;
    expander_set_pins(expander_dev_handle2, expander_pins);
    vTaskDelay(pdMS_TO_TICKS(100)); // Give GT911 time to boot up
}

void touch_init(){
    // Initialize touch IO (I2C)
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(global_bus_handle, &io_config, &io_handle));

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
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(io_handle, &tp_cfg, &touch_handle));
}

void display_init(){
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
}