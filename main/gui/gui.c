#include "gui.h"

// Logging
static const char *TAG = "Boat Assistant: GUI";

// Timer callback function checked periodically by LVGL
void backlight_check_timer_cb(lv_timer_t * timer) {
    static bool backlight_is_on = true;
    
    // Get inactive time from the default display
    // Note: For LVGL v8, use: uint32_t idle_time = lv_disp_get_inactive_time(NULL);
    uint32_t idle_time = lv_display_get_inactive_time(lv_display_get_default());

    if (idle_time >= BACKLIGHT_TIMEOUT_MS) {
        if (backlight_is_on) {
            expander_pins = set_backlight_state(false, expander_pins, expander_dev_handle2); // Turn off backlight
            backlight_is_on = false;
        }
    } else {
        if (!backlight_is_on) {
            expander_pins = set_backlight_state(true, expander_pins, expander_dev_handle2);  // Turn back on if there is user activity
            backlight_is_on = true;
        }
    }
}

void screen_init(lv_display_t *disp) {
        // Start the adapter task
    ESP_ERROR_CHECK(esp_lv_adapter_start());
     // Draw with LVGL (guarded by adapter lock for thread safety)
    if (esp_lv_adapter_lock(-1) == ESP_OK) {
        // Get the base screen
        lv_obj_t *base_screen = lv_display_get_screen_active(disp);

        // Create tileview to house "screens"
        lv_obj_t *tileview = lv_tileview_create(base_screen);
        lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(100));
        lv_obj_align(tileview, LV_ALIGN_CENTER, 0, 0);

        // Create tiles to act as screens
        // First row is settings
        lv_obj_t *screen_nmea_settings = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT| LV_DIR_BOTTOM);
        lv_obj_t *screen_settings = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT| LV_DIR_BOTTOM);
        lv_obj_t *screen_wifi = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_LEFT | LV_DIR_BOTTOM);
        // Second Row is data screens
        lv_obj_t *screen_nmea = lv_tileview_add_tile(tileview, 0, 1, LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_t *screen_environment = lv_tileview_add_tile(tileview, 1, 1, LV_DIR_LEFT | LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_t *screen_victron = lv_tileview_add_tile(tileview, 2, 1, LV_DIR_LEFT | LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_t *screen_fuel = lv_tileview_add_tile(tileview, 3, 1, LV_DIR_LEFT);
        // Set default screen
        lv_tileview_set_tile_by_index(tileview, 1, 1, LV_ANIM_OFF);
        
        // Status bar for all screens
        lv_obj_t * status_bar = lv_obj_create(base_screen);
        lv_obj_set_size(status_bar, LV_HOR_RES, 25);
        lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_set_style_bg_color(status_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
        //lv_obj_set_style_bg_color(status_bar, lv_color_hex(0xF7F7F7), LV_PART_MAIN);
        lv_obj_set_style_border_width(status_bar, 0, 0);
        lv_obj_set_style_pad_all(status_bar, 4, 0);
        lv_obj_set_style_radius(status_bar, 0, 0);
        lv_obj_set_layout(status_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(status_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(status_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        // Wifi Status label
        //lv_obj_t * container_wifi = lv_obj_create(status_bar);
        //lv_obj_remove_style_all(container_wifi);
        //lv_obj_set_size(container_wifi, lv_pct(30),lv_pct(100));
        //lv_obj_set_flex_flow(container_wifi, LV_FLEX_FLOW_ROW);
        //lv_obj_set_flex_align(container_wifi, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        //    lv_obj_t * lbl_wifi = lv_label_create(container_wifi);
        //    lv_label_set_text(lbl_wifi, "Wifi: ");
        //    lv_obj_t * lbl_netstatus = lv_label_create(container_wifi);
        //    lv_label_bind_text(lbl_netstatus, state_system_wifi_status_text, NULL);
        // Memory Free label
        lv_obj_t * container_memory = lv_obj_create(status_bar);
        lv_obj_remove_style_all(container_memory);
        lv_obj_set_size(container_memory, lv_pct(30),lv_pct(100));
        lv_obj_set_flex_flow(container_memory, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_memory, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_t * lbl_wifi = lv_label_create(container_memory);
            lv_label_set_text(lbl_wifi, "Free Memory: ");
            lv_obj_t * lbl_netstatus = lv_label_create(container_memory);
            lv_label_bind_text(lbl_netstatus, state_system_memory_free_text, NULL);
        // Inside Temperature label
        lv_obj_t * container_temp_in = lv_obj_create(status_bar);
        lv_obj_remove_style_all(container_temp_in);
        lv_obj_set_size(container_temp_in, lv_pct(15),lv_pct(100));
        lv_obj_set_flex_flow(container_temp_in, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_temp_in, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_t * lbl_inside = lv_label_create(container_temp_in);
            lv_label_set_text(lbl_inside, "Inside: ");
            lv_obj_t * lbl_in_temp = lv_label_create(container_temp_in);
            lv_label_bind_text(lbl_in_temp, state_ruuvi_tag_1_temperature_text, NULL);
        // Outside Temperature label
        lv_obj_t * container_temp_out = lv_obj_create(status_bar);
        lv_obj_remove_style_all(container_temp_out);
        lv_obj_set_size(container_temp_out, lv_pct(15),lv_pct(100));
        lv_obj_set_flex_flow(container_temp_out, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_temp_out, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_t * lbl_outside = lv_label_create(container_temp_out);
            lv_label_set_text(lbl_outside, "Outside: ");
            lv_obj_t * lbl_out_temp = lv_label_create(container_temp_out);
            lv_label_bind_text(lbl_out_temp, state_ruuvi_tag_2_temperature_text, NULL);
        // Fuel Remaining label
        lv_obj_t * container_fuel = lv_obj_create(status_bar);
        lv_obj_remove_style_all(container_fuel);
        lv_obj_set_size(container_fuel, lv_pct(15),lv_pct(100));
        lv_obj_set_flex_flow(container_fuel, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_fuel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_t * lbl_fuel = lv_label_create(container_fuel);
            lv_label_set_text(lbl_fuel, "Fuel: ");
            lv_obj_t * lbl_fuel_level = lv_label_create(container_fuel);
            lv_label_bind_text(lbl_fuel_level, state_victron_dieseltank_level_percent_text, NULL);
        // Battery Level label
        lv_obj_t * container_batt = lv_obj_create(status_bar);
        lv_obj_remove_style_all(container_batt);
        lv_obj_set_size(container_batt, lv_pct(15),lv_pct(100));
        lv_obj_set_flex_flow(container_batt, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_batt, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_t * lbl_battery = lv_label_create(container_batt);
            lv_label_set_text(lbl_battery, "Battery: ");
            lv_obj_t * lbl_batt_soc = lv_label_create(container_batt);
            lv_label_bind_text(lbl_batt_soc, state_victron_house_battery_soc_text, NULL);


        // Lay out all screens
        // First row
        screen_nmea_settings_layout(screen_nmea_settings);
        screen_settings_layout(screen_settings);
        screen_wifi_layout(screen_wifi);
        // Second row
        screen_nmea_layout(screen_nmea);
        screen_environment_layout(screen_environment);
        screen_victron_layout(screen_victron);
        screen_fuel_layout(screen_fuel);

        // Finish and show everything
        esp_lv_adapter_unlock();
    }
}