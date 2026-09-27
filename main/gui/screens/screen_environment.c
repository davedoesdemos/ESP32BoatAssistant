#include "screen_environment.h"

static char hum_in_buf[16] = "---%";
static char hum_in_prev_buf[16] = "---%";
static char hum_out_buf[16] = "---%";
static char hum_out_prev_buf[16] = "---%";
static char temp_in_buf[16] = "---°C";
static char temp_in_prev_buf[16] = "---°C";
static char temp_out_buf[16] = "---°C";
static char temp_out_prev_buf[16] = "---°C";

static lv_subject_t *sensor_hum_in;
static lv_subject_t *sensor_hum_in_text;
static lv_subject_t *sensor_hum_out;
static lv_subject_t *sensor_hum_out_text;
static lv_subject_t *sensor_temp_in;
static lv_subject_t *sensor_temp_in_text;
static lv_subject_t *sensor_temp_out;
static lv_subject_t *sensor_temp_out_text;

void init_environment_subjects(){
        //init sensor subjects
        lv_subject_init_int(sensor_hum_in, 0);
        lv_subject_init_int(sensor_hum_out, 0);
        lv_subject_init_int(sensor_temp_in, 0);
        lv_subject_init_int(sensor_temp_out, 0);

        sensor_hum_in_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_hum_in_text, hum_in_buf, hum_in_prev_buf, sizeof(hum_in_buf));
        lv_subject_set_string(sensor_hum_in_text, "---%");

        sensor_hum_out_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_hum_out_text, hum_out_buf, hum_out_prev_buf, sizeof(hum_out_buf));
        lv_subject_set_string(sensor_hum_out_text, "---%");

        sensor_temp_in_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_temp_in_text, temp_in_buf, temp_in_prev_buf, sizeof(temp_in_buf));
        lv_subject_set_string(sensor_temp_in_text, "---°C");

        sensor_temp_out_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_temp_out_text, temp_out_buf, temp_out_prev_buf, sizeof(temp_out_buf));
        lv_subject_set_string(sensor_temp_out_text, "---°C");
}

//dynamically update sensor data for label
void update_temp_in(int new_value) {
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", new_value);
    lv_subject_set_int(sensor_temp_in, new_value);
    lv_subject_set_string(sensor_temp_in_text, temp);
}
void update_temp_out(int new_value) {
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", new_value);
    //printf("update temp out %d\n", new_value);
    lv_subject_set_int(sensor_temp_out, new_value);
    lv_subject_set_string(sensor_temp_out_text, temp);
}
void update_hum_in(int new_value) {
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", new_value);
    lv_subject_set_int(sensor_hum_in, new_value);
    lv_subject_set_string(sensor_hum_in_text, temp);
}
void update_hum_out(int new_value) {
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", new_value);
    lv_subject_set_int(sensor_hum_out, new_value);
    lv_subject_set_string(sensor_hum_out_text, temp);
}

void screen_environment_layout(lv_obj_t *screen_environment) {
    init_environment_subjects();

    lv_obj_t *container_left = lv_obj_create(screen_environment);
    lv_obj_remove_style_all(container_left); // Remove background/borders for a clean look
    lv_obj_set_size(container_left, 280, 300);
    lv_obj_align(container_left, LV_ALIGN_CENTER, -150, 0);

    // 2. Outer Arc: Temperature (Size: 150x150)
    lv_obj_t *temp_arc1 = lv_arc_create(container_left);
    lv_obj_set_size(temp_arc1, 270, 270);
    lv_obj_set_style_pad_all(temp_arc1, 0, LV_PART_MAIN);
    lv_obj_align(temp_arc1, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_rotation(temp_arc1, 135);     // Start from bottom-left
    lv_arc_set_bg_angles(temp_arc1, 0, 270); // 270-degree partial circle
    lv_arc_set_range(temp_arc1, -10, 50);    // Temp range e.g., -10°C to 50°C

    lv_arc_bind_value(temp_arc1, sensor_temp_in);
    //lv_arc_set_value(temp_arc1, 22);         // Example value: 22°C

    // Style the Temperature Arc (Red theme)
    lv_obj_set_style_arc_width(temp_arc1, 25, 0);
    lv_obj_set_style_arc_width(temp_arc1, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(temp_arc1, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(temp_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

    // 3. Inner Arc: Humidity (Size: 110x110)
    lv_obj_t *humid_arc1 = lv_arc_create(container_left);
    lv_obj_set_size(humid_arc1, 190, 190);
    lv_obj_align(humid_arc1, LV_ALIGN_CENTER, 0, -5); // Adjust slightly upward to sit inside nicely
    lv_arc_set_rotation(humid_arc1, 135);
    lv_arc_set_bg_angles(humid_arc1, 0, 270);
    lv_arc_set_range(humid_arc1, 0, 100);    // Humidity range 0-100%

    lv_arc_bind_value(humid_arc1, sensor_hum_in);
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
    lv_label_bind_text(temp_in_label, sensor_temp_in_text, NULL);
    //lv_label_set_text(temp_in_label, "22C");
    lv_obj_set_style_text_align(temp_in_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp_in_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temp_in_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *hum_in_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(hum_in_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(hum_in_label, sensor_hum_in_text, NULL);
    //lv_label_set_text(hum_in_label, "22C");
    lv_obj_set_style_text_align(hum_in_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(hum_in_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(hum_in_label, &lv_font_montserrat_24, 0); 

    // 5. Bottom Title Label (e.g., "Room 1")
    lv_obj_t *title_label1 = lv_label_create(container_left);
    lv_label_set_text(title_label1, "Indoor");
    lv_obj_set_style_text_font(title_label1, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_label1, LV_ALIGN_BOTTOM_MID, 0, 0);


    lv_obj_t *container_right = lv_obj_create(screen_environment);
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

    lv_arc_bind_value(temp_arc2, sensor_temp_out);
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

    lv_arc_bind_value(humid_arc2, sensor_hum_out);
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
    lv_label_bind_text(temp_out_label, sensor_temp_out_text, NULL);
    //lv_label_set_text(temp_out_label, "22C");
    lv_obj_set_style_text_align(temp_out_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp_out_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temp_out_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *hum_out_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(hum_out_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(hum_out_label, sensor_hum_out_text, NULL);
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