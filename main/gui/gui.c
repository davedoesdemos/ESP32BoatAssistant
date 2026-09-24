#include "gui.h"

//logging
static const char *TAG = "boat assistant gui";
static lv_obj_t * lbl_netstatus;

// Set Status Label
void set_netstatus(const char * status) {
    lv_label_set_text(lbl_netstatus, status);
}

// Timer callback function checked periodically by LVGL
void backlight_check_timer_cb(lv_timer_t * timer) {
    static bool backlight_is_on = true;
    
    // Get inactive time from the default display
    // Note: For LVGL v8, use: uint32_t idle_time = lv_disp_get_inactive_time(NULL);
    uint32_t idle_time = lv_display_get_inactive_time(lv_display_get_default());

    if (idle_time >= BACKLIGHT_TIMEOUT_MS) {
        if (backlight_is_on) {
            printf("backlight on");
            expander_pins = set_backlight_state(false, expander_pins, expander_dev_handle2); // Turn off backlight
            backlight_is_on = false;
        }
    } else {
        if (!backlight_is_on) {
            printf("backlight off");
            expander_pins = set_backlight_state(true, expander_pins, expander_dev_handle2);  // Turn back on if there is user activity
            backlight_is_on = true;
        }
    }
}

void screen_init(lv_display_t *disp) {
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
        lv_obj_t *screen_settings = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT| LV_DIR_BOTTOM);
        lv_obj_t *screen_wifi = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_BOTTOM);
        lv_obj_t *screen_nmea = lv_tileview_add_tile(tileview, 0, 1, LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_t *screen_environment = lv_tileview_add_tile(tileview, 1, 1, LV_DIR_LEFT | LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_t *screen_2 = lv_tileview_add_tile(tileview, 2, 1, LV_DIR_LEFT);

        lv_tileview_set_tile_by_index(tileview, 0, 1, LV_ANIM_OFF);
        
        //init sensor reading sub
        lv_subject_init_int(&sensor_reading_subj, 0);
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
        lbl_netstatus = lv_label_create(status_bar);
        lv_label_set_text(lbl_netstatus, "Not Connected");
        //Clock label
        lv_obj_t * lbl_time = lv_label_create(status_bar);
        lv_label_set_text(lbl_time, "12:00");

        //Lay out screens
        //First row
        screen_settings_layout(screen_settings);
        screen_wifi_layout(screen_wifi);
        //Second row
        screen_nmea_layout(screen_nmea);
        screen_environment_layout(screen_environment);
        screen_2_layout(screen_2);

        //finish and show everything
        esp_lv_adapter_unlock();
    }
}