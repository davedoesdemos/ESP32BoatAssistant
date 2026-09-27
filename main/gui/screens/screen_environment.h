#ifndef SCREEN_ENVIRONMENT_H
#define SCREEN_ENVIRONMENT_H

#include "lvgl.h"
#include <stdio.h>
#include "screen_nmea.h"

void update_temp_in(int new_value);
void update_temp_out(int new_value);
void update_hum_in(int new_value);
void update_hum_out(int new_value);
void screen_environment_layout(lv_obj_t *screen_environment);

#endif //SCREEN_ENVIRONMENT_H