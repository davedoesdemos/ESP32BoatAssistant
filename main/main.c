#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include "esp_check.h"        // Dependent header file
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_gt911.h"  // Example with GT911
#include "driver/i2c_master.h"
#include "rgblcd43b.h"
#include "esp_lv_adapter.h"  // Includes display & input adapters
#include "i2c.h"
#include "expander.h"
#include "backlight.h"

static lv_subject_t sensor_reading_subj;
uint8_t expander_pins = 0x00;

static const char *TAG = "button app";

//CH422 Expander pin masks for Waveshare 4.3b
#define TOUCH_RESET_PIN_MASK (1 << 1)
#define BACKLIGHT_PIN_MASK (1 << 2)
#define DISPLAY_RESET_PIN_MASK (1<<3)

static lv_obj_t * slider_label;

typedef struct {
    i2c_master_bus_handle_t global_bus_handle;
    i2c_master_dev_handle_t expander_dev_handle;
    i2c_master_dev_handle_t expander_dev_handle2;
}  i2c_config;

i2c_config i2c_settings = {NULL, NULL, NULL};

//dynamically update sensor data for label
void update_sensor_data(int new_value) {
    lv_subject_set_int(&sensor_reading_subj, new_value);
}


//Event callback triggered when the slider value changes
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t seconds = 0;
    /* Get current slider position (which represents seconds) */
    int32_t value_sec = lv_slider_get_value(slider);

    if(value_sec <= 50) {
            // Linear map from 10s to 60s over the first half of the slider
            seconds = 10 + (value_sec * (60 - 10) / 50);
        } else {
            // Linear map from 60s to 300s over the second half of the slider
            seconds = 60 + ((value_sec - 50) * (300 - 60) / 50);
        }
    
    /* Dynamic calculation: Update your system variable when user moves the knob */
    BACKLIGHT_TIMEOUT_MS = seconds * 1000;
    
    /* Dynamically format and update the label text string */
    char buf[32];
    snprintf(buf, sizeof(buf), "%d s", (int)seconds);
    lv_label_set_text(slider_label, buf);
    
    /* Realign the label beneath the slider */
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

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

static void reset_btn_text_timer_cb(lv_timer_t * timer)
{
    // Retrieve the label pointer we passed into the timer user_data
    lv_obj_t * label = (lv_obj_t *)lv_timer_get_user_data(timer);
    
    if (label != NULL) {
        lv_label_set_text(label, "Click Me");
    }
    
    // Delete the timer so it only fires once
    lv_timer_del(timer);
}

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);

    // Check if the button was clicked/released
    if(code == LV_EVENT_CLICKED) {
        // Get the label object which is the first child of the button
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        if (label != NULL) {
            lv_label_set_text(label, "Touched!");
            // 2. Create a one-shot timer to revert the text after 2000 milliseconds (2 seconds)
            lv_timer_t * timer = lv_timer_create(reset_btn_text_timer_cb, 2000, label);
            
            // 3. Configure the timer to only run once
            lv_timer_set_repeat_count(timer, 1);
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

    //Start the adapter task
    ESP_ERROR_CHECK(esp_lv_adapter_start());

    //Draw with LVGL (guarded by adapter lock for thread safety)
    if (esp_lv_adapter_lock(-1) == ESP_OK) {
        //get the base screen
        lv_obj_t *base_screen = lv_display_get_screen_active(disp);

        //create tileview
        lv_obj_t *tileview = lv_tileview_create(base_screen);
        lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(100));
        lv_obj_align(tileview, LV_ALIGN_CENTER, 0, 0);

        //create tiles to act as screens
        lv_obj_t *screen1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
        lv_obj_t *screen2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
        lv_obj_t *screen3 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_LEFT | LV_DIR_BOTTOM);
        lv_obj_t *screen4 = lv_tileview_add_tile(tileview, 2, 1, LV_DIR_TOP);

        //ALL SCREENS
        //Create a status bar
        lv_obj_t * status_bar = lv_obj_create(base_screen);
        lv_obj_set_size(status_bar, LV_HOR_RES, 25);
        lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x0099FF), LV_PART_MAIN);
        lv_obj_set_style_border_width(status_bar, 0, 0);
        lv_obj_set_style_pad_all(status_bar, 4, 0);
        lv_obj_set_style_radius(status_bar, 0, 0);
        lv_obj_set_layout(status_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(status_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        //Status label
        lv_obj_t * lbl_title = lv_label_create(status_bar);
        lv_label_set_text(lbl_title, "Device OK");
        //Clock label
        lv_obj_t * lbl_time = lv_label_create(status_bar);
        lv_label_set_text(lbl_time, "12:00");

        //SCREEN 1
        //Create a button on the first screen
        lv_obj_t * btn = lv_btn_create(screen1);
        lv_obj_set_size(btn, 150, 60);
        lv_obj_center(btn);
        
        //Assign the touch/click event callback to the button
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

        //Create a label inside the button
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "Click Me");
        lv_obj_center(label);

        lv_subject_init_int(&sensor_reading_subj, 0);

        //SCREEN 2
        lv_obj_t *labelsensor = lv_label_create(screen2);
        lv_label_bind_text(labelsensor, &sensor_reading_subj, "Sensor: %d PSI");
        //lv_label_set_text(labelsensor, "Hello Jude!");
        lv_obj_center(labelsensor);

        //SCREEN 3 - Settings

        lv_obj_t *label3 = lv_label_create(screen3);
        lv_label_set_text(label3, "Backlight Timeout");
        lv_obj_center(label3);
        lv_obj_t * slider = lv_slider_create(screen3);
        lv_obj_set_width(slider, 300);                          /* Set width in pixels */
        lv_obj_center(slider);                                  /* Align perfectly to display center */
        lv_slider_set_range(slider, 0, 100);                    /* Min value: 0, Max value: 100 */
        lv_slider_set_value(slider, 0, LV_ANIM_OFF);
        lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        slider_label = lv_label_create(screen3);
        //set the label
        int32_t initial_seconds = BACKLIGHT_TIMEOUT_MS / 1000;
        char buf[32];
        snprintf(buf, sizeof(buf), "%d s", (int)initial_seconds);
        lv_label_set_text(slider_label, buf);
        lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        //SCREEN 4 for temp and humidity
        lv_obj_t *container_left = lv_obj_create(screen4);
        lv_obj_remove_style_all(container_left); // Remove background/borders for a clean look
        lv_obj_set_size(container_left, 180, 200);
        lv_obj_align(container_left, LV_ALIGN_CENTER, -90, 0);

        // 2. Outer Arc: Temperature (Size: 150x150)
        lv_obj_t *temp_arc1 = lv_arc_create(container_left);
        lv_obj_set_size(temp_arc1, 150, 150);
        lv_obj_set_style_pad_all(temp_arc1, 0, LV_PART_MAIN);
        lv_obj_align(temp_arc1, LV_ALIGN_TOP_MID, 0, 10);
        lv_arc_set_rotation(temp_arc1, 135);     // Start from bottom-left
        lv_arc_set_bg_angles(temp_arc1, 0, 270); // 270-degree partial circle
        lv_arc_set_range(temp_arc1, -10, 50);    // Temp range e.g., -10°C to 50°C
        
        //lv_label_bind_text(labelsensor, &sensor_reading_subj, "Sensor: %d PSI");
        lv_arc_bind_value(temp_arc1, &sensor_reading_subj);
        //lv_arc_set_value(temp_arc1, 22);         // Example value: 22°C
    
        // Style the Temperature Arc (Red theme)
        lv_obj_set_style_arc_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_remove_flag(temp_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

        // 3. Inner Arc: Humidity (Size: 110x110)
        lv_obj_t *humid_arc1 = lv_arc_create(container_left);
        lv_obj_set_size(humid_arc1, 110, 110);
        lv_obj_align(humid_arc1, LV_ALIGN_CENTER, 0, -15); // Adjust slightly upward to sit inside nicely
        lv_arc_set_rotation(humid_arc1, 135);
        lv_arc_set_bg_angles(humid_arc1, 0, 270);
        lv_arc_set_range(humid_arc1, 0, 100);    // Humidity range 0-100%
        lv_arc_set_value(humid_arc1, 60);         // Example value: 60%
        
        // Style the Humidity Arc (Blue theme)
        lv_obj_set_style_arc_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_remove_flag(humid_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

        // 4. Centred Text Label for values
        lv_obj_t *val_label1 = lv_label_create(container_left);
        lv_label_set_text_fmt(val_label1, "#ff0000 22°C#\n#0000ff 60%%#");
        lv_obj_set_style_text_align(val_label1, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_recolor(val_label1, true); // Allows using #hex color# inline
        lv_obj_align(val_label1, LV_ALIGN_CENTER, 0, -15);

        // 5. Bottom Title Label (e.g., "Room 1")
        lv_obj_t *title_label1 = lv_label_create(container_left);
        lv_label_set_text(title_label1, "Indoor");
        lv_obj_align(title_label1, LV_ALIGN_BOTTOM_MID, 0, 0);


        lv_obj_t *container_right = lv_obj_create(screen4);
        lv_obj_remove_style_all(container_right); // Remove background/borders for a clean look
        lv_obj_set_size(container_right, 180, 200);
        lv_obj_align(container_right, LV_ALIGN_CENTER, 90, 0);

        // 2. Outer Arc: Temperature (Size: 150x150)
        lv_obj_t *temp_arc2 = lv_arc_create(container_right);
        lv_obj_set_size(temp_arc2, 150, 150);
        lv_obj_set_style_pad_all(temp_arc2, 0, LV_PART_MAIN);
        lv_obj_align(temp_arc2, LV_ALIGN_TOP_MID, 0, 10);
        lv_arc_set_rotation(temp_arc2, 135);     // Start from bottom-left
        lv_arc_set_bg_angles(temp_arc2, 0, 270); // 270-degree partial circle
        lv_arc_set_range(temp_arc2, -10, 50);    // Temp range e.g., -10°C to 50°C
        lv_arc_set_value(temp_arc2, 28);         // Example value: 22°C
    
        // Style the Temperature Arc (Red theme)
        lv_obj_set_style_arc_color(temp_arc2, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(temp_arc2, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_remove_flag(temp_arc2, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

        // 3. Inner Arc: Humidity (Size: 110x110)
        lv_obj_t *humid_arc2 = lv_arc_create(container_right);
        lv_obj_set_size(humid_arc2, 110, 110);
        lv_obj_align(humid_arc2, LV_ALIGN_CENTER, 0, -15); // Adjust slightly upward to sit inside nicely
        lv_arc_set_rotation(humid_arc2, 135);
        lv_arc_set_bg_angles(humid_arc2, 0, 270);
        lv_arc_set_range(humid_arc2, 0, 100);    // Humidity range 0-100%
        lv_arc_set_value(humid_arc2, 88);         // Example value: 60%
        
        // Style the Humidity Arc (Blue theme)
        lv_obj_set_style_arc_color(humid_arc2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(humid_arc2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_remove_flag(humid_arc2, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

        // 4. Centred Text Label for values
        lv_obj_t *val_label2 = lv_label_create(container_right);
        lv_label_set_text_fmt(val_label2, "#ff0000 28°C#\n#0000ff 88%%#");
        lv_obj_set_style_text_align(val_label2, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_recolor(val_label2, true); // Allows using #hex color# inline
        lv_obj_align(val_label2, LV_ALIGN_CENTER, 0, -15);

        // 5. Bottom Title Label (e.g., "Room 1")
        lv_obj_t *title_label2 = lv_label_create(container_right);
        lv_label_set_text(title_label2, "Outdoor");
        lv_obj_align(title_label2, LV_ALIGN_BOTTOM_MID, 0, 0);

 
        //finish and show everything
        esp_lv_adapter_unlock();
    }
    
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
