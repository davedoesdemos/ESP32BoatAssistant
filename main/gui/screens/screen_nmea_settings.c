#include "screen_nmea_settings.h"

static lv_obj_t * spinbox_depth_offset;

static void lv_spinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox_depth_offset);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox_depth_offset);
    }
}

void screen_nmea_settings_layout(lv_obj_t *screen_nmea_settings) {
    spinbox_depth_offset = lv_spinbox_create(screen_nmea_settings);
    lv_spinbox_set_range(spinbox_depth_offset, 0, 50);
    lv_spinbox_set_digit_count(spinbox_depth_offset, 3);
    lv_spinbox_set_dec_point_pos(spinbox_depth_offset, 1);
    lv_spinbox_step_prev(spinbox_depth_offset);
    lv_obj_set_width(spinbox_depth_offset, 100);
    lv_obj_center(spinbox_depth_offset);

    lv_spinbox_bind_value(spinbox_depth_offset, state_nmea_boat_depth_offset);

    lv_obj_t * lbl_depth_offset = lv_label_create(screen_nmea_settings);
    lv_label_set_text(lbl_depth_offset, "Depth Offset");
    lv_obj_center(lbl_depth_offset);
    lv_obj_set_y( lbl_depth_offset, -50 );
    lv_obj_set_style_text_font(lbl_depth_offset, &lv_font_montserrat_24, 0); 

    int32_t h = lv_obj_get_height(spinbox_depth_offset);

    lv_obj_t * btn_up = lv_button_create(screen_nmea_settings);
    lv_obj_set_size(btn_up, h, h);
    lv_obj_align_to(btn_up, spinbox_depth_offset, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(btn_up, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn_up, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  NULL);

    lv_obj_t * btn_down = lv_button_create(screen_nmea_settings);
    lv_obj_set_size(btn_down, h, h);
    lv_obj_align_to(btn_down, spinbox_depth_offset, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(btn_down, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn_down, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);
}