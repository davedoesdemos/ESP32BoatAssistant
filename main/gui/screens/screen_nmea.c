#include "screen_nmea.h"

// Allocate buffer sizes large enough for your data strings
static char sog_buf[16] = "---kt";
static char sog_prev_buf[16] = "---kt";
static char cog_buf[16] = "---°";
static char cog_prev_buf[16] = "---°";
static char tws_buf[16] = "---kt";
static char tws_prev_buf[16] = "---kt";
static char twd_buf[16] = "---°";
static char twd_prev_buf[16] = "---°";
static char aws_buf[16] = "---kt";
static char aws_prev_buf[16] = "---kt";
static char awd_buf[16] = "---°";
static char awd_prev_buf[16] = "---°";
static char depth_buf[16] = "---m";
static char depth_prev_buf[16] = "---m";
static char hum_in_buf[16] = "---%";
static char hum_in_prev_buf[16] = "---%";
static char hum_out_buf[16] = "---%";
static char hum_out_prev_buf[16] = "---%";
static char fuel_level_buf[16] = "---°";
static char fuel_level_prev_buf[16] = "---°";
static char fuel_capacity_buf[16] = "---°";
static char fuel_capacity_prev_buf[16] = "---°";
static char fuel_percent_buf[16] = "---°";
static char fuel_percent_prev_buf[16] = "---°";

// Declare your LVGL subjects
static lv_subject_t * subj_sog;
static lv_subject_t * subj_cog;
static lv_subject_t * subj_tws;
static lv_subject_t * subj_twd;
static lv_subject_t * subj_aws;
static lv_subject_t * subj_awd;
static lv_subject_t * subj_depth;
static lv_subject_t * subj_fuel_level;
static lv_subject_t * subj_fuel_capacity;
static lv_subject_t * subj_fuel_percent;
// Initialize them during your setup / main function

void init_nmea_subjects(void) {
    subj_sog = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_sog, sog_buf, sog_prev_buf, sizeof(sog_buf));
    lv_subject_set_string(subj_sog, "---kt");

    subj_cog = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_cog, cog_buf, cog_prev_buf, sizeof(cog_buf));
    lv_subject_set_string(subj_cog, "---°");

    subj_tws = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_tws, tws_buf, tws_prev_buf, sizeof(tws_buf));
    lv_subject_set_string(subj_tws, "---kt");

    subj_twd = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_twd, twd_buf, twd_prev_buf, sizeof(twd_buf));
    lv_subject_set_string(subj_twd, "---°");

    subj_aws = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_aws, aws_buf, aws_prev_buf, sizeof(aws_buf));
    lv_subject_set_string(subj_aws, "---kt");

    subj_awd = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_awd, awd_buf, awd_prev_buf, sizeof(awd_buf));
    lv_subject_set_string(subj_awd, "---°");

    subj_depth = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_depth, depth_buf, depth_prev_buf, sizeof(depth_buf));
    lv_subject_set_string(subj_depth, "---m");

    subj_fuel_level = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_fuel_level, fuel_level_buf, fuel_level_prev_buf, sizeof(fuel_level_buf));
    lv_subject_set_string(subj_fuel_level, "---l");

    subj_fuel_capacity = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_fuel_capacity, fuel_capacity_buf, fuel_capacity_prev_buf, sizeof(fuel_capacity_buf));
    lv_subject_set_string(subj_fuel_capacity, "---l");

    subj_fuel_percent = lv_subject_create(LV_SUBJECT_TYPE_STRING);
    lv_subject_set_string_buffer_static(subj_fuel_percent, fuel_percent_buf, fuel_percent_prev_buf, sizeof(fuel_percent_buf));
    lv_subject_set_string(subj_fuel_percent, "---%");
}

// update function to have queue reader and process into labels
void update_nmea() {
        char cog_str_temp[16];
        char sog_str_temp[16];
        char tws_str_temp[16];
        char twd_str_temp[16];
        char aws_str_temp[16];
        char awd_str_temp[16];
        char depth_str_temp[16];
        char fuel_level_str_temp[16];
        char fuel_capacity_str_temp[16];
        char fuel_percent_str_temp[16];
    nmea_msg_t received_nmea = { .type = UI_UPDATE_NULL, .data.int_val = 0};
    while (1) {
        // Block indefinitely until an item arrives in the queue
        if (xQueueReceive(msg_queue_nmea, &received_nmea, portMAX_DELAY) == pdTRUE) {
            switch (received_nmea.type) {
                case UI_UPDATE_COG:
                    snprintf(cog_str_temp, sizeof(cog_str_temp), "%.1f°", received_nmea.data.float_val);
                    //ESP_LOGW("RECEIVER", "Received value: %s on Core %d", cog_str_temp, xPortGetCoreID());
                    lv_lock();
                    lv_subject_set_string(subj_cog, cog_str_temp);
                    lv_unlock();
                    break;
                case UI_UPDATE_SOG:
                    snprintf(sog_str_temp, sizeof(sog_str_temp), "%.1f kt", received_nmea.data.float_val);
                    //ESP_LOGW("RECEIVER", "Received value: %s on Core %d", sog_str_temp, xPortGetCoreID());
                    lv_lock(); //xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
                    lv_subject_set_string(subj_sog, sog_str_temp);
                    lv_unlock(); //xSemaphoreGive(lvgl_mutex);
                    break;
                case UI_UPDATE_TWS:
                    snprintf(tws_str_temp, sizeof(tws_str_temp), "%.1f kt", received_nmea.data.float_val);
                    lv_lock(); //xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
                    lv_subject_set_string(subj_tws, tws_str_temp);
                    lv_unlock(); //xSemaphoreGive(lvgl_mutex);
                    break;
                case UI_UPDATE_TWD:
                    snprintf(twd_str_temp, sizeof(twd_str_temp), "%.1f°", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_twd, twd_str_temp);
                    lv_unlock();
                    break;
                case UI_UPDATE_AWS:
                    snprintf(aws_str_temp, sizeof(aws_str_temp), "%.1f kt", received_nmea.data.float_val);
                    lv_lock(); //xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
                    lv_subject_set_string(subj_aws, aws_str_temp);
                    lv_unlock(); //xSemaphoreGive(lvgl_mutex);
                    break;
                case UI_UPDATE_AWD:
                    snprintf(awd_str_temp, sizeof(awd_str_temp), "%.1f°", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_awd, awd_str_temp);
                    lv_unlock();
                    break;
                case UI_UPDATE_DEPTH:
                    snprintf(depth_str_temp, sizeof(depth_str_temp), "%.1fm", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_depth, depth_str_temp);
                    lv_unlock();
                    break;
                case UI_UPDATE_FUELLEVEL:
                    snprintf(fuel_level_str_temp, sizeof(fuel_level_str_temp), "%.1fl", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_fuel_level, fuel_level_str_temp);
                    update_fuel_level((int)round(received_nmea.data.float_val));
                    lv_unlock();
                    break;
                case UI_UPDATE_FUELCAPACITY:
                    snprintf(fuel_capacity_str_temp, sizeof(fuel_capacity_str_temp), "%.1fl", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_fuel_capacity, fuel_capacity_str_temp);
                    update_fuel_capacity((int)round(received_nmea.data.float_val));
                    lv_unlock();
                    break;
                case UI_UPDATE_FUELPERCENT:
                    snprintf(fuel_percent_str_temp, sizeof(fuel_percent_str_temp), "%.1fl", received_nmea.data.float_val);
                    lv_lock();
                    lv_subject_set_string(subj_fuel_percent, fuel_percent_str_temp);
                    update_fuel_percent((int)round(received_nmea.data.float_val));
                    lv_unlock();
                    break;
                default:
                    // code block
                    break;
            }
            
        }
    }
}

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

        create_data_box(row0_container, "Depth", subj_depth, 0xF7F7F7);
        create_data_box(row0_container, "SOG", subj_sog, 0xF7F7F7);
        create_data_box(row0_container, "COG", subj_cog, 0xF7F7F7);
        create_data_box(row0_container, "Fuel", subj_fuel_level, 0xF7F7F7);

        // Row 1
        lv_obj_t * row1_container = lv_obj_create(column1_container);
        lv_obj_remove_style_all(row1_container);
        lv_obj_set_layout(row1_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_width(row1_container, lv_pct(100));

        create_data_box(row1_container, "TWS", subj_tws, 0xF7F7F7);
        create_data_box(row1_container, "TWD", subj_twd, 0xF7F7F7);
        create_data_box(row1_container, "AWS", subj_aws, 0xF7F7F7);
        create_data_box(row1_container, "AWD", subj_awd, 0xF7F7F7);

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