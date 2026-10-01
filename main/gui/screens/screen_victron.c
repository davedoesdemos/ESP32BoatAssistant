#include "screen_victron.h"

void screen_victron_layout(lv_obj_t *screen_victron) {
    lv_obj_t *container_left = lv_obj_create(screen_victron);
    lv_obj_remove_style_all(container_left); // Remove background/borders for a clean look
    lv_obj_set_size(container_left, 280, 300);
    lv_obj_align(container_left, LV_ALIGN_CENTER, -150, 0);

    // Outer Arc: Temperature (Size: 150x150)
    lv_obj_t *temp_arc1 = lv_arc_create(container_left);
    lv_obj_set_size(temp_arc1, 270, 270);
    lv_obj_set_style_pad_all(temp_arc1, 0, LV_PART_MAIN);
    lv_obj_align(temp_arc1, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_rotation(temp_arc1, 135);     // Start from bottom-left
    lv_arc_set_bg_angles(temp_arc1, 0, 270); // 270-degree partial circle
    lv_arc_set_range(temp_arc1, -10, 50);    // Temp range e.g., -10°C to 50°C

    lv_arc_bind_value(temp_arc1, state_victron_house_battery_soc);

    // Style the Temperature Arc (Red theme)
    lv_obj_set_style_arc_width(temp_arc1, 25, 0);
    lv_obj_set_style_arc_width(temp_arc1, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(temp_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

    // Inner Arc: Humidity
    lv_obj_t *humid_arc1 = lv_arc_create(container_left);
    lv_obj_set_size(humid_arc1, 190, 190);
    lv_obj_align(humid_arc1, LV_ALIGN_CENTER, 0, -5); // Adjust slightly upward to sit inside nicely
    lv_arc_set_rotation(humid_arc1, 135);
    lv_arc_set_bg_angles(humid_arc1, 0, 270);
    lv_arc_set_range(humid_arc1, 0, 100);    // Humidity range 0-100%

    lv_arc_bind_value(humid_arc1, state_victron_house_battery_power);
    //lv_arc_set_value(humid_arc1, 60);         // Example value: 60%
        
    // Style the Humidity Arc (Blue theme)
    lv_obj_set_style_arc_width(humid_arc1, 25, 0);
    lv_obj_set_style_arc_width(humid_arc1, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(humid_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

    // Centred Text Labels for values
    lv_obj_t *temp_in_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(temp_in_label, lv_color_hex(0xFF0000), 0);

    lv_label_bind_text(temp_in_label, state_victron_house_battery_soc_text, NULL);

    lv_obj_set_style_text_align(temp_in_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp_in_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temp_in_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *hum_in_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(hum_in_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(hum_in_label, state_victron_house_battery_power_text, NULL);
    //lv_label_set_text(hum_in_label, "22C");
    lv_obj_set_style_text_align(hum_in_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(hum_in_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(hum_in_label, &lv_font_montserrat_24, 0); 

    // 5. Bottom Title Label (e.g., "Room 1")
    lv_obj_t *title_label1 = lv_label_create(container_left);
    lv_label_set_text(title_label1, "Indoor");
    lv_obj_set_style_text_font(title_label1, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_label1, LV_ALIGN_BOTTOM_MID, 0, 0);


    lv_obj_t *container_right = lv_obj_create(screen_victron);
    lv_obj_remove_style_all(container_right); // Remove background/borders for a clean look
    lv_obj_set_size(container_right, 280, 300);
    lv_obj_align(container_right, LV_ALIGN_CENTER, 150, 0);

    // 2. Outer Arc: Temperature (Size: 150x150)
    lv_obj_t *temp_arc2 = lv_arc_create(container_right);
    lv_obj_set_size(temp_arc2, 270, 270);
    lv_obj_set_style_pad_all(temp_arc2, 0, LV_PART_MAIN);
    lv_obj_align(temp_arc2, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_rotation(temp_arc2, 135);     // Start from bottom-left
    lv_arc_set_bg_angles(temp_arc2, 0, 270); // 270-degree partial circle
    lv_arc_set_range(temp_arc2, -10, 50);    // Temp range e.g., -10°C to 50°C

    lv_arc_bind_value(temp_arc2, state_ruuvi_tag_2_temperature);
    //lv_arc_set_value(temp_arc2, 28);         // Example value: 22°C

    // Style the Temperature Arc (Red theme)
    lv_obj_set_style_arc_width(temp_arc2, 25, 0);
    lv_obj_set_style_arc_width(temp_arc2, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc2, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(temp_arc2, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(temp_arc2, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

    // 3. Inner Arc: Humidity (Size: 110x110)
    lv_obj_t *humid_arc2 = lv_arc_create(container_right);
    lv_obj_set_size(humid_arc2, 190, 190);
    lv_obj_align(humid_arc2, LV_ALIGN_CENTER, 0, -5); // Adjust slightly upward to sit inside nicely
    lv_arc_set_rotation(humid_arc2, 135);
    lv_arc_set_bg_angles(humid_arc2, 0, 270);
    lv_arc_set_range(humid_arc2, 0, 100);    // Humidity range 0-100%

    lv_arc_bind_value(humid_arc2, state_ruuvi_tag_2_humidity);
    //lv_arc_set_value(humid_arc2, 88);         // Example value: 60%
        
    // Style the Humidity Arc (Blue theme)
    lv_obj_set_style_arc_width(humid_arc2, 25, 0);
    lv_obj_set_style_arc_width(humid_arc2, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(humid_arc2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(humid_arc2, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(humid_arc2, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

    // Centred Text Labels for values
    lv_obj_t *temp_out_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(temp_out_label, lv_color_hex(0xFF0000), 0);
    lv_label_bind_text(temp_out_label, state_ruuvi_tag_2_temperature_text, NULL);
    //lv_label_set_text(temp_out_label, "22C");
    lv_obj_set_style_text_align(temp_out_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp_out_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temp_out_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *hum_out_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(hum_out_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(hum_out_label, state_ruuvi_tag_2_humidity_text, NULL);
    //lv_label_set_text(hum_out_label, "22C");
    lv_obj_set_style_text_align(hum_out_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(hum_out_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(hum_out_label, &lv_font_montserrat_24, 0); 

    // 5. Bottom Title Label (e.g., "Room 1")
    lv_obj_t *title_label2 = lv_label_create(container_right);
    lv_label_set_text(title_label2, "Outdoor");
    lv_obj_set_style_text_font(title_label2, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_label2, LV_ALIGN_BOTTOM_MID, 0, 0);
}