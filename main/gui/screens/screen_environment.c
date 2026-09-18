#include "screen_environment.h"

lv_subject_t sensor_reading_subj;

//dynamically update sensor data for label
void update_sensor_data(int new_value) {
    lv_subject_set_int(&sensor_reading_subj, new_value);
}

void screen_environment_layout(lv_obj_t *screen_environment) {

    lv_obj_t *container_left = lv_obj_create(screen_environment);
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


    lv_obj_t *container_right = lv_obj_create(screen_environment);
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

 
}