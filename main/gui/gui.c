#include "gui.h"

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
        lv_obj_t *screen_1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
        lv_obj_t *screen_environment = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
        lv_obj_t *screen_settings = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_LEFT | LV_DIR_BOTTOM);
        lv_obj_t *screen_2 = lv_tileview_add_tile(tileview, 2, 1, LV_DIR_TOP);
        
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
        lv_obj_t * lbl_title = lv_label_create(status_bar);
        lv_label_set_text(lbl_title, "Device OK");
        //Clock label
        lv_obj_t * lbl_time = lv_label_create(status_bar);
        lv_label_set_text(lbl_time, "12:00");

        //SCREEN 1
        screen_1_layout(screen_1);
        //SCREEN 2
        screen_2_layout(screen_2);
        //SCREEN 3 - Settings
        screen_settings_layout(screen_settings);
        //SCREEN 4 for temp and humidity
        screen_environment_layout(screen_environment);

        //finish and show everything
        esp_lv_adapter_unlock();
    }
}