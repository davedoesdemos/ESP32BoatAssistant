#include "screen_wifi.h"

// UI Element Handles
static lv_obj_t * ssid_dropdown;
static lv_obj_t * password_ta;
static lv_obj_t * keyboard;
static lv_obj_t * rescan_btn;
static lv_obj_t * connect_btn;

static void connect_btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        char ssid[64];
        lv_dropdown_get_selected_str(ssid_dropdown, ssid, sizeof(ssid));
        const char * password = lv_textarea_get_text(password_ta);

        wifi_credentials_t *creds = malloc(sizeof(wifi_credentials_t));
            strlcpy(creds->ssid, ssid, sizeof(creds->ssid));
            strlcpy(creds->password, password, sizeof(creds->password));

        // Launch the task asynchronously without blocking LVGL
        xTaskCreate(wifi_task, "wifi_task", 4096, creds, 5, NULL);
    }
}

static void rescan_btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        char *scanned_ssids = wifi_scan();
        lv_dropdown_set_options(ssid_dropdown, scanned_ssids);
        //printf("Scanned SSIDs:\n%s", scanned_ssids);
        free(scanned_ssids); // Crucial to prevent ESP32 memory leaks
        //char *scanned_ssids = wifi_scan();
        // TRIGGER BACKGROUND HARDWARE WI-FI MANAGER HERE
        // Example: WiFi.begin(ssid, password);
    }
}

void screen_wifi_layout(lv_obj_t *screen_wifi){
    // Layout Columns
    lv_obj_t * column1_container = lv_obj_create(screen_wifi);
    lv_obj_remove_style_all(column1_container);
    lv_obj_set_height(column1_container, 200);
    lv_obj_set_width(column1_container, lv_pct(100));
    lv_obj_set_y( column1_container, 40 );
    lv_obj_set_flex_flow(column1_container, LV_FLEX_FLOW_COLUMN);
    // Empty Row for spacing
    lv_obj_t * row0_container = lv_obj_create(column1_container);
    lv_obj_remove_style_all(row0_container);
    lv_obj_set_height(row0_container, 50);
    // Layout Row 1
    lv_obj_t * row1_container = lv_obj_create(column1_container);
    lv_obj_remove_style_all(row1_container);
    lv_obj_set_height(row1_container, 50);
    lv_obj_set_layout(row1_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_width(row1_container, lv_pct(100));


    // Wi-Fi SSID Dropdown
    ssid_dropdown = lv_dropdown_create(row1_container);
    lv_obj_set_width(ssid_dropdown, 260);
    lv_dropdown_set_options(ssid_dropdown, "Scanning networks...\nHit Rescan"); // Placeholder text

    // Rescan Button
    rescan_btn = lv_button_create(row1_container);
    lv_obj_set_width( rescan_btn, 100);
    lv_obj_add_event_cb(rescan_btn, rescan_btn_event_cb, LV_EVENT_ALL, NULL);
        // Button Label
        lv_obj_t * rescan_btn_label = lv_label_create(rescan_btn);
        lv_label_set_text(rescan_btn_label, "Rescan");
        lv_obj_center(rescan_btn_label);

    // Layout Row 2
    lv_obj_t * row2_container = lv_obj_create(column1_container);
    lv_obj_remove_style_all(row2_container);
    lv_obj_set_height(row2_container, 50);
    lv_obj_set_layout(row2_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row2_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_width(row2_container, lv_pct(100));

    // Password Text Area
    password_ta = lv_textarea_create(row2_container);
    lv_obj_set_width(password_ta, 260);
    lv_obj_set_height( password_ta, LV_SIZE_CONTENT);   /// 36
    lv_textarea_set_password_mode(password_ta, true);
    lv_textarea_set_one_line(password_ta,true);
    lv_textarea_set_placeholder_text(password_ta, "Enter Password");
    //lv_obj_add_event_cb(password_ta, ta_event_cb, LV_EVENT_ALL, NULL);

    // Connect Button
    connect_btn = lv_btn_create(row2_container);
    lv_obj_set_width(connect_btn, 100);
    lv_obj_add_event_cb(connect_btn, connect_btn_event_cb, LV_EVENT_ALL, NULL);
        // Button Label
        lv_obj_t * connect_btn_label = lv_label_create(connect_btn);
        lv_label_set_text(connect_btn_label, "Connect");
        lv_obj_center(connect_btn_label);

    // Empty Row for spacing
    lv_obj_t * row3_container = lv_obj_create(column1_container);
    lv_obj_remove_style_all(row3_container);
    lv_obj_set_height(row3_container, 50);

    // On screen keyboard
    keyboard = lv_keyboard_create(screen_wifi);
    lv_keyboard_set_textarea(keyboard, password_ta);
    lv_obj_set_width(keyboard, lv_pct(100));
    lv_obj_set_height(keyboard, lv_pct(50));
    lv_obj_set_align(keyboard, LV_ALIGN_BOTTOM_MID );
}