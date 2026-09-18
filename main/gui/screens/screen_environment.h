#ifndef SCREEN_ENVIRONMENT_H
#define SCREEN_ENVIRONMENT_H

#include "lvgl.h"

extern lv_subject_t sensor_reading_subj;

void update_sensor_data(int new_value);
void screen_environment_layout(lv_obj_t *screen_environment);

#endif //SCREEN_ENVIRONMENT_H