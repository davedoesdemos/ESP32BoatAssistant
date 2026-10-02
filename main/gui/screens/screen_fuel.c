#include "screen_fuel.h"

static lv_obj_t * create_data_box(lv_obj_t * parent, const char * title, lv_subject_t * subject_value, uint32_t bg_hex_color) {
    // Create and reset container
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);

    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);    
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

void screen_fuel_layout(lv_obj_t *screen_fuel) {
    // Layout row for three columns
    lv_obj_t * row1_container = lv_obj_create(screen_fuel);
    lv_obj_remove_style_all(row1_container);
    lv_obj_set_height(row1_container, 450);
    lv_obj_set_width(row1_container, lv_pct(100));
    lv_obj_set_y( row1_container, 28 );
    lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

        // column 0
        lv_obj_t * column0_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column0_container);
        lv_obj_set_layout(column0_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column0_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column0_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column0_container, lv_pct(100));

            // Bar: Fuel (Size: 150x150)
            lv_obj_t *fuel_bar = lv_bar_create(column0_container);
            lv_obj_set_size(fuel_bar, 80, lv_pct(90));
            lv_obj_set_style_pad_all(fuel_bar, 0, LV_PART_MAIN);
            lv_bar_set_range(fuel_bar, 0, 100);

            lv_bar_bind_value(fuel_bar, state_victron_dieseltank_level_percent);

            lv_obj_set_style_bg_color(fuel_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
            lv_obj_remove_flag(fuel_bar, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)
            lv_obj_set_style_radius(fuel_bar, 0, LV_PART_MAIN);
            lv_obj_set_style_radius(fuel_bar, 0, LV_PART_INDICATOR);

        // column 1
        lv_obj_t * column1_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column1_container);
        lv_obj_set_layout(column1_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column1_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column1_container, lv_pct(100));

            create_data_box(column1_container, "Level", state_victron_dieseltank_level_litres_text, 0xF7F7F7);
            create_data_box(column1_container, "Percent", state_victron_dieseltank_level_percent_text, 0xF7F7F7);

        // column 2
        lv_obj_t * column2_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column2_container);
        lv_obj_set_layout(column2_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column2_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column2_container, lv_pct(100));

            create_data_box(column2_container, "Time", state_victron_dieseltank_remaining_hours_text, 0xF7F7F7);
            create_data_box(column2_container, "Miles", state_victron_dieseltank_remaining_miles_text, 0xF7F7F7);
}