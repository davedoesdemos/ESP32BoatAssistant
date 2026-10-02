#include "screen_environment.h"

void screen_environment_layout(lv_obj_t *screen_environment) {
    // Left Container
    lv_obj_t *container_left = lv_obj_create(screen_environment);
    lv_obj_remove_style_all(container_left); // Remove background/borders for a clean look
    lv_obj_set_size(container_left, 280, 300);
    lv_obj_align(container_left, LV_ALIGN_CENTER, -150, 0);

    // Outer Arc: Temperature Inside
    lv_obj_t *temperature_inside_arc1 = lv_arc_create(container_left);
    lv_obj_set_size(temperature_inside_arc1, 270, 270);
    lv_obj_set_style_pad_all(temperature_inside_arc1, 0, LV_PART_MAIN);
    lv_obj_align(temperature_inside_arc1, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_rotation(temperature_inside_arc1, 135);     // Start from bottom-left
    lv_arc_set_bg_angles(temperature_inside_arc1, 0, 270); // 270-degree partial circle
    lv_arc_set_range(temperature_inside_arc1, -10, 50);    // Temp range e.g., -10°C to 50°C

    lv_arc_bind_value(temperature_inside_arc1, state_ruuvi_tag_1_temperature);

    // Style
    lv_obj_set_style_arc_width(temperature_inside_arc1, 25, 0);
    lv_obj_set_style_arc_width(temperature_inside_arc1, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temperature_inside_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(temperature_inside_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(temperature_inside_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

    // Inner Arc: Humidity
    lv_obj_t *humidity_inside_arc = lv_arc_create(container_left);
    lv_obj_set_size(humidity_inside_arc, 190, 190);
    lv_obj_align(humidity_inside_arc, LV_ALIGN_CENTER, 0, -5); // Adjust slightly upward to sit inside nicely
    lv_arc_set_rotation(humidity_inside_arc, 135);
    lv_arc_set_bg_angles(humidity_inside_arc, 0, 270);
    lv_arc_set_range(humidity_inside_arc, 0, 100);    // Humidity range 0-100%

    lv_arc_bind_value(humidity_inside_arc, state_ruuvi_tag_1_humidity);
        
    // Style
    lv_obj_set_style_arc_width(humidity_inside_arc, 25, 0);
    lv_obj_set_style_arc_width(humidity_inside_arc, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(humidity_inside_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(humidity_inside_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(humidity_inside_arc, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

    // Centred Text Labels for values
    lv_obj_t *temperature_inside_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(temperature_inside_label, lv_color_hex(0xFF0000), 0);
    lv_label_bind_text(temperature_inside_label, state_ruuvi_tag_1_temperature_text, NULL);
    lv_obj_set_style_text_align(temperature_inside_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temperature_inside_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temperature_inside_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *humidity_inside_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(humidity_inside_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(humidity_inside_label, state_ruuvi_tag_1_humidity_text, NULL);
    //lv_label_set_text(hum_in_label, "22C");
    lv_obj_set_style_text_align(humidity_inside_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(humidity_inside_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(humidity_inside_label, &lv_font_montserrat_24, 0); 

    // Title Label
    lv_obj_t *title_inside_label = lv_label_create(container_left);
    lv_label_set_text(title_inside_label, "Indoor");
    lv_obj_set_style_text_font(title_inside_label, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_inside_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Right Container
    lv_obj_t *container_right = lv_obj_create(screen_environment);
    lv_obj_remove_style_all(container_right); // Remove background/borders for a clean look
    lv_obj_set_size(container_right, 280, 300);
    lv_obj_align(container_right, LV_ALIGN_CENTER, 150, 0);

    // Outer Arc: Temperature
    lv_obj_t *temperature_outside_arc = lv_arc_create(container_right);
    lv_obj_set_size(temperature_outside_arc, 270, 270);
    lv_obj_set_style_pad_all(temperature_outside_arc, 0, LV_PART_MAIN);
    lv_obj_align(temperature_outside_arc, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_rotation(temperature_outside_arc, 135);     // Start from bottom-left
    lv_arc_set_bg_angles(temperature_outside_arc, 0, 270); // 270-degree partial circle
    lv_arc_set_range(temperature_outside_arc, -10, 50);    // Temp range e.g., -10°C to 50°C

    lv_arc_bind_value(temperature_outside_arc, state_ruuvi_tag_2_temperature);

    // Style
    lv_obj_set_style_arc_width(temperature_outside_arc, 25, 0);
    lv_obj_set_style_arc_width(temperature_outside_arc, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temperature_outside_arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(temperature_outside_arc, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(temperature_outside_arc, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

    // Inner Arc: Humidity
    lv_obj_t *humidity_outside_arc = lv_arc_create(container_right);
    lv_obj_set_size(humidity_outside_arc, 190, 190);
    lv_obj_align(humidity_outside_arc, LV_ALIGN_CENTER, 0, -5);
    lv_arc_set_rotation(humidity_outside_arc, 135);
    lv_arc_set_bg_angles(humidity_outside_arc, 0, 270);
    lv_arc_set_range(humidity_outside_arc, 0, 100);

    lv_arc_bind_value(humidity_outside_arc, state_ruuvi_tag_2_humidity);
        
    // Style
    lv_obj_set_style_arc_width(humidity_outside_arc, 25, 0);
    lv_obj_set_style_arc_width(humidity_outside_arc, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(humidity_outside_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(humidity_outside_arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(humidity_outside_arc, LV_OBJ_FLAG_CLICKABLE);

    // Centred Text Labels for values
    lv_obj_t *temperature_outside_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(temperature_outside_label, lv_color_hex(0xFF0000), 0);
    lv_label_bind_text(temperature_outside_label, state_ruuvi_tag_2_temperature_text, NULL);
    lv_obj_set_style_text_align(temperature_outside_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temperature_outside_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temperature_outside_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *humidity_outside_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(humidity_outside_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(humidity_outside_label, state_ruuvi_tag_2_humidity_text, NULL);
    lv_obj_set_style_text_align(humidity_outside_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(humidity_outside_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(humidity_outside_label, &lv_font_montserrat_24, 0); 

    // Title Label
    lv_obj_t *title_outside_label = lv_label_create(container_right);
    lv_label_set_text(title_outside_label, "Outdoor");
    lv_obj_set_style_text_font(title_outside_label, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_outside_label, LV_ALIGN_BOTTOM_MID, 0, 0);
}