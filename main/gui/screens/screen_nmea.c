#include "screen_nmea.h"

static lv_obj_t * create_data_box(lv_obj_t * parent, const char * title, lv_subject_t * subject_value, uint32_t bg_hex_color) {
    // Create and reset container
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);

    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);    
    //lv_obj_set_width(container, lv_pct(100));
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, lv_color_hex(bg_hex_color), LV_PART_MAIN);
        // Create and populate labels
        lv_obj_t * lbl_title = lv_label_create(container);
        lv_label_set_text(lbl_title, title);
        lv_obj_set_width(lbl_title, lv_pct(100));
        lv_obj_set_style_text_align(lbl_title, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_24, 0); 
        
        lv_obj_t * lbl_value = lv_label_create(container);
        //lv_label_set_text(lbl_value, value);
        lv_obj_set_width(lbl_title, lv_pct(100));
        lv_obj_set_style_text_align(lbl_value, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(lbl_value, &lv_font_montserrat_34, 0); 

        lv_label_bind_text(lbl_value, subject_value, NULL);


    return container;
}

void screen_nmea_layout(lv_obj_t *screen_nmea){
    // Layout Column for three rows of 4 labels
    lv_obj_t * column1_container = lv_obj_create(screen_nmea);
    lv_obj_remove_style_all(column1_container);
    lv_obj_set_height(column1_container, 450);
    lv_obj_set_width(column1_container, lv_pct(100));
    lv_obj_set_y( column1_container, 28 );
    lv_obj_set_flex_flow(column1_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(column1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

        // Row 0
        lv_obj_t * row0_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row0_container);
        lv_obj_set_layout(row0_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row0_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row0_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row0_container, lv_pct(100));

        create_data_box(row0_container, "Depth", state_nmea_boat_depth_text, 0xF7F7F7);
        create_data_box(row0_container, "SOG", state_nmea_boat_speed_over_ground_text, 0xF7F7F7);
        create_data_box(row0_container, "COG", state_nmea_boat_course_over_ground_text, 0xF7F7F7);
        create_data_box(row0_container, "Fuel", state_nmea_dieseltank_level_litres_text, 0xF7F7F7);

        // Row 1
        lv_obj_t * row1_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row1_container);
        lv_obj_set_layout(row1_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row1_container, lv_pct(100));

        create_data_box(row1_container, "TWS", state_nmea_wind_true_speed_text, 0xF7F7F7);
        create_data_box(row1_container, "TWD", state_nmea_wind_true_direction_text, 0xF7F7F7);
        create_data_box(row1_container, "AWS", state_nmea_wind_apparent_speed_text, 0xF7F7F7);
        create_data_box(row1_container, "AWD", state_nmea_wind_apparent_direction_text, 0xF7F7F7);

        // Row 2
        lv_obj_t * row2_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row2_container);
        lv_obj_set_layout(row2_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row2_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row2_container, lv_pct(100));


        create_data_box(row2_container, "Hum Inside", state_ruuvi_tag_1_humidity_text, 0xF7F7F7);
        create_data_box(row2_container, "Inside", state_ruuvi_tag_1_temperature_text, 0xF7F7F7);
        create_data_box(row2_container, "Hum Outside", state_ruuvi_tag_2_humidity_text, 0xF7F7F7);
        create_data_box(row2_container, "Outside", state_ruuvi_tag_2_temperature_text, 0xF7F7F7);
}