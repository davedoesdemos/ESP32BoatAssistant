#include "screen_nmea.h"

// Allocate buffer sizes large enough for your data strings
static char sog_buf[16] = "0.0 kn";
static char sog_prev_buf[16] = "0.0 kn";
static char cog_buf[16] = "000°";
static char cog_prev_buf[16] = "000°";

// Declare your LVGL subjects
static lv_subject_t * subj_sog;
static lv_subject_t * subj_cog;
// Initialize them during your setup / main function

void init_nmea_subjects(void) {
    subj_sog = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_sog, sog_buf, sog_prev_buf, sizeof(sog_buf));
    lv_subject_set_string(subj_sog, "0.0 kn");

    subj_cog = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_cog, cog_buf, cog_prev_buf, sizeof(cog_buf));
    lv_subject_set_string(subj_cog, "000°");
}

// update function to have queue reader and process into labels
void update_nmea(float speed, float course) {
    char temp[16];

    // Format and push new values to the subjects
    snprintf(temp, sizeof(temp), "%.1f kn", speed);
    lv_subject_set_string(subj_sog, temp); // The UI updates automatically!

    snprintf(temp, sizeof(temp), "%03.0f°", course);
    lv_subject_set_string(subj_cog, temp); // The UI updates automatically!
}

lv_obj_t * create_data_box(lv_obj_t * parent, const char * title, lv_subject_t * subject_value, uint32_t bg_hex_color) {
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
        lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_14, 0); 
        
        lv_obj_t * lbl_value = lv_label_create(container);
        //lv_label_set_text(lbl_value, value);
        lv_obj_set_width(lbl_title, lv_pct(100));
        lv_obj_set_style_text_align(lbl_value, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(lbl_value, &lv_font_montserrat_24, 0); 

        // BIND THE SUBJECT: Automatically updates label text when the subject changes
        if (subject_value != NULL) {
            lv_label_bind_text(lbl_value, subject_value, NULL); // Third parameter is an optional printf format string if needed
        } else {
            lv_label_set_text(lbl_value, "---"); // Fallback if no subject is assigned
        }

    return container;
}

void screen_nmea_layout(lv_obj_t *screen_nmea){
    init_nmea_subjects();
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

        create_data_box(row0_container, "SOG", subj_sog, 0xF7F7F7);
//        create_data_box(row0_container, "Depth", "4.5 kn", 0xF7F7F7);
//        create_data_box(row0_container, "STW", "3.5 kn", 0xF7F7F7);
//        create_data_box(row0_container, "STW", "3.5 kn", 0xF7F7F7);

        // Row 1
        lv_obj_t * row1_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row1_container);
        lv_obj_set_layout(row1_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row1_container, lv_pct(100));

//        create_data_box(row1_container, "Wind Dir", "220", 0xF7F7F7);
//        create_data_box(row1_container, "Depth", "4.5 kn", 0xF7F7F7);
//        create_data_box(row1_container, "SSTW", "3.5 kn", 0xF7F7F7);
//        create_data_box(row1_container, "STW", "3.5 kn", 0xF7F7F7);

        // Row 2
        lv_obj_t * row2_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row2_container);
        lv_obj_set_layout(row2_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row2_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row2_container, lv_pct(100));

//        create_data_box(row2_container, "Wind Speed", "14.5 kn", 0xF7F7F7);
//        create_data_box(row2_container, "Heading", "180", 0xF7F7F7);
//        create_data_box(row2_container, "COG", "170", 0xF7F7F7);
//        create_data_box(row2_container, "STW", "3.5 kn", 0xF7F7F7);
}