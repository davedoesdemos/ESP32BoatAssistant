#include "screen_2.h"

void screen_2_layout(lv_obj_t *screen_2){
        lv_obj_t *labelsensor = lv_label_create(screen_2);
        //lv_label_bind_text(labelsensor, &sensor_reading_subj, "Sensor: %d PSI");
        lv_label_set_text(labelsensor, "Hello Jude!");
        lv_obj_center(labelsensor);
}