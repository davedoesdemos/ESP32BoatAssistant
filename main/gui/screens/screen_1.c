#include "screen_1.h"

static void reset_btn_text_timer_cb(lv_timer_t * timer)
{
    // Retrieve the label pointer we passed into the timer user_data
    lv_obj_t * label = (lv_obj_t *)lv_timer_get_user_data(timer);
    
    if (label != NULL) {
        lv_label_set_text(label, "Click Me");
    }
    
    // Delete the timer so it only fires once
    lv_timer_del(timer);
}

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);

    // Check if the button was clicked/released
    if(code == LV_EVENT_CLICKED) {
        // Get the label object which is the first child of the button
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        if (label != NULL) {
            lv_label_set_text(label, "Touched!");
            // 2. Create a one-shot timer to revert the text after 2000 milliseconds (2 seconds)
            lv_timer_t * timer = lv_timer_create(reset_btn_text_timer_cb, 2000, label);
            
            // 3. Configure the timer to only run once
            lv_timer_set_repeat_count(timer, 1);
        }
    }
}

void screen_1_layout(lv_obj_t *screen_1) {
    //Create a button on the first screen
    lv_obj_t * btn = lv_btn_create(screen_1);
    lv_obj_set_size(btn, 150, 60);
    lv_obj_center(btn);
    
    //Assign the touch/click event callback to the button
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

    //Create a label inside the button
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Click Me");
    lv_obj_center(label);
}