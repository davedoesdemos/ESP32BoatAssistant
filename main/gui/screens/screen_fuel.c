#include "screen_fuel.h"

static char fuel_level_buf[16] = "---l";
static char fuel_level_prev_buf[16] = "---l";
static char fuel_capacity_buf[16] = "---l";
static char fuel_capacity_prev_buf[16] = "---l";
static char fuel_percent_buf[16] = "---%";
static char fuel_percent_prev_buf[16] = "---%";
static char fuel_time_buf[16] = "---hrs";
static char fuel_time_prev_buf[16] = "---hrs";
static char fuel_miles_buf[16] = "---NM";
static char fuel_miles_prev_buf[16] = "---NM";

static lv_subject_t *sensor_fuel_level;
static lv_subject_t *sensor_fuel_level_text;
static lv_subject_t *sensor_fuel_capacity;
static lv_subject_t *sensor_fuel_capacity_text;
static lv_subject_t *sensor_fuel_percent;
static lv_subject_t *sensor_fuel_percent_text;
static lv_subject_t *sensor_fuel_time_text;
static lv_subject_t *sensor_fuel_miles_text;

void init_fuel_subjects(){
        //init sensor subjects
        sensor_fuel_level = lv_subject_create(LV_SUBJECT_TYPE_INT);
        lv_subject_set_int(sensor_fuel_level, 0);
        
        sensor_fuel_capacity = lv_subject_create(LV_SUBJECT_TYPE_INT);
        lv_subject_set_int(sensor_fuel_capacity, 0);

        sensor_fuel_percent = lv_subject_create(LV_SUBJECT_TYPE_INT);
        lv_subject_set_int(sensor_fuel_percent, 0);

        sensor_fuel_level_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_fuel_level_text, fuel_level_buf, fuel_level_prev_buf, sizeof(fuel_level_buf));
        lv_subject_set_string(sensor_fuel_level_text, "---%");

        sensor_fuel_capacity_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_fuel_capacity_text, fuel_capacity_buf, fuel_capacity_prev_buf, sizeof(fuel_capacity_buf));
        lv_subject_set_string(sensor_fuel_capacity_text, "---%");

        sensor_fuel_percent_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_fuel_percent_text, fuel_percent_buf, fuel_percent_prev_buf, sizeof(fuel_percent_buf));
        lv_subject_set_string(sensor_fuel_percent_text, "---°C");

        sensor_fuel_time_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_fuel_time_text, fuel_time_buf, fuel_time_prev_buf, sizeof(fuel_time_buf));
        lv_subject_set_string(sensor_fuel_time_text, "---°C");

        sensor_fuel_miles_text = lv_subject_create(LV_SUBJECT_TYPE_STRING);
        lv_subject_set_string_buffer_static(sensor_fuel_miles_text, fuel_miles_buf, fuel_miles_prev_buf, sizeof(fuel_miles_buf));
        lv_subject_set_string(sensor_fuel_miles_text, "---°C");
}

//dynamically update sensor data for label
void update_fuel_level(int new_value) {
    char temp[16];
    int fuel_time = new_value / 2;
    int fuel_miles = fuel_time * 5;
    snprintf(temp, sizeof(temp), "%dl", new_value);
    lv_subject_set_int(sensor_fuel_level, new_value);
    lv_subject_set_string(sensor_fuel_level_text, temp);
    snprintf(temp, sizeof(temp), "%dhrs", fuel_time);
    lv_subject_set_string(sensor_fuel_time_text, temp);
    snprintf(temp, sizeof(temp), "%dNM", fuel_miles);
    lv_subject_set_string(sensor_fuel_miles_text, temp);
}
void update_fuel_capacity(int new_value) {
    char temp[16];
    snprintf(temp, sizeof(temp), "%dl", new_value);
    //printf("update temp out %d\n", new_value);
    lv_subject_set_int(sensor_fuel_capacity, new_value);
    lv_subject_set_string(sensor_fuel_capacity_text, temp);
}
void update_fuel_percent(int new_value) {
    lv_subject_set_int(sensor_fuel_percent, new_value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", new_value);
    lv_subject_set_string(sensor_fuel_percent_text, temp);
}

static lv_obj_t * create_data_box(lv_obj_t * parent, const char * title, lv_subject_t * subject_value, uint32_t bg_hex_color) {
    // Create and reset container
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);

    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);    
    //lv_obj_set_height(container, 50);
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

        // BIND THE SUBJECT: Automatically updates label text when the subject changes
        if (subject_value != NULL) {
            lv_label_bind_text(lbl_value, subject_value, NULL); // Third parameter is an optional printf format string if needed
        } else {
            lv_label_set_text(lbl_value, "---"); // Fallback if no subject is assigned
        }

    return container;
}

void screen_fuel_layout(lv_obj_t *screen_fuel) {
    init_fuel_subjects();

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

            lv_bar_bind_value(fuel_bar, sensor_fuel_percent);
            //lv_bar_set_value(fuel_bar, 34, LV_ANIM_ON);

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

            create_data_box(column1_container, "Level", sensor_fuel_level_text, 0xF7F7F7);
            create_data_box(column1_container, "Percent", sensor_fuel_percent_text, 0xF7F7F7);

        // column 2
        lv_obj_t * column2_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column2_container);
        lv_obj_set_layout(column2_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column2_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_height(column2_container, lv_pct(100));

            create_data_box(column2_container, "Time", sensor_fuel_time_text, 0xF7F7F7);
            create_data_box(column2_container, "Miles", sensor_fuel_miles_text, 0xF7F7F7);

    ///////////////////////////////////////////////////
/*
    lv_obj_t *container_left = lv_obj_create(screen_fuel);
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

    lv_arc_bind_value(temp_arc1, sensor_fuel_percent);
    //lv_arc_set_value(temp_arc1, 22);         // Example value: 22°C

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

    lv_arc_bind_value(humid_arc1, sensor_fuel_level);
    //lv_arc_set_value(humid_arc1, 60);         // Example value: 60%
        
    // Style the Humidity Arc (Blue theme)
    lv_obj_set_style_arc_width(humid_arc1, 25, 0);
    lv_obj_set_style_arc_width(humid_arc1, 25, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(humid_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_flag(humid_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only

    // Centred Text Labels for values
    lv_obj_t *fuel_percent_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(fuel_percent_label, lv_color_hex(0xFF0000), 0);
    lv_label_bind_text(fuel_percent_label, sensor_fuel_percent_text, NULL);
    //lv_label_set_text(fuel_percent_label, "22C");
    lv_obj_set_style_text_align(fuel_percent_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(fuel_percent_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(fuel_percent_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *fuel_level_label = lv_label_create(container_left);
    lv_obj_set_style_text_color(fuel_level_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(fuel_level_label, sensor_fuel_level_text, NULL);
    //lv_label_set_text(fuel_level_label, "22C");
    lv_obj_set_style_text_align(fuel_level_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(fuel_level_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(fuel_level_label, &lv_font_montserrat_24, 0); 

    // 5. Bottom Title Label (e.g., "Room 1")
    lv_obj_t *title_label1 = lv_label_create(container_left);
    lv_label_set_text(title_label1, "Indoor");
    lv_obj_set_style_text_font(title_label1, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_label1, LV_ALIGN_BOTTOM_MID, 0, 0);


    lv_obj_t *container_right = lv_obj_create(screen_fuel);
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

    //lv_arc_bind_value(temp_arc2, sensor_temp_out);
    lv_arc_set_value(temp_arc2, 28);         // Example value: 22°C

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

    lv_arc_bind_value(humid_arc2, sensor_fuel_capacity);
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
    //lv_label_bind_text(temp_out_label, sensor_temp_out_text, NULL);
    lv_label_set_text(temp_out_label, "22C");
    lv_obj_set_style_text_align(temp_out_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp_out_label, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_text_font(temp_out_label, &lv_font_montserrat_24, 0); 

    lv_obj_t *fuel_capacity_label = lv_label_create(container_right);
    lv_obj_set_style_text_color(fuel_capacity_label, lv_color_hex(0x0000FF), 0);
    lv_label_bind_text(fuel_capacity_label, sensor_fuel_capacity_text, NULL);
    //lv_label_set_text(fuel_capacity_label, "22C");
    lv_obj_set_style_text_align(fuel_capacity_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(fuel_capacity_label, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_style_text_font(fuel_capacity_label, &lv_font_montserrat_24, 0); 

    // 5. Bottom Title Label (e.g., "Room 1")
    lv_obj_t *title_label2 = lv_label_create(container_right);
    lv_label_set_text(title_label2, "Outdoor");
    lv_obj_set_style_text_font(title_label2, &lv_font_montserrat_24, 0); 
    lv_obj_align(title_label2, LV_ALIGN_BOTTOM_MID, 0, 0);
    */
}