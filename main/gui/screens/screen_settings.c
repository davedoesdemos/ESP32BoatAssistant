#include "screen_settings.h"
#include "lvgl.h"  // Includes display & input adapters
#include "backlight.h"

static lv_obj_t * slider_label;

//Event callback triggered when the slider value changes
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t seconds = 0;
    /* Get current slider position (which represents seconds) */
    int32_t value_sec = lv_slider_get_value(slider);

    if(value_sec <= 50) {
            // Linear map from 10s to 60s over the first half of the slider
            seconds = 10 + (value_sec * (60 - 10) / 50);
        } else {
            // Linear map from 60s to 300s over the second half of the slider
            seconds = 60 + ((value_sec - 50) * (300 - 60) / 50);
        }
    
    /* Dynamic calculation: Update your system variable when user moves the knob */
    BACKLIGHT_TIMEOUT_MS = seconds * 1000;
    
    /* Dynamically format and update the label text string */
    char buf[32];
    snprintf(buf, sizeof(buf), "%d s", (int)seconds);
    lv_label_set_text(slider_label, buf);
    
    /* Realign the label beneath the slider */
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

void screen_settings_layout(lv_obj_t *screen_settings) {
    lv_obj_t *label3 = lv_label_create(screen_settings);
    lv_label_set_text(label3, "Backlight Timeout");
    lv_obj_center(label3);
    lv_obj_t * slider = lv_slider_create(screen_settings);
    lv_obj_set_width(slider, 400);                          /* Set width in pixels */
    lv_obj_center(slider);                                  /* Align perfectly to display center */
    lv_slider_set_range(slider, 0, 100);                    /* Min value: 0, Max value: 100 */
    lv_slider_set_value(slider, 0, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    slider_label = lv_label_create(screen_settings);
    //set the label
    int32_t initial_seconds = BACKLIGHT_TIMEOUT_MS / 1000;
    char buf[32];
    snprintf(buf, sizeof(buf), "%d s", (int)initial_seconds);
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}