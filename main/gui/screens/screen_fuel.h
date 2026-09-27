#ifndef SCREEN_FUEL_H
#define SCREEN_FUEL_H

#include "lvgl.h"
#include <stdio.h>
#include "screen_nmea.h"

void update_fuel_level(int new_value);
void update_fuel_capacity(int new_value);
void update_fuel_percent(int new_value);

void screen_fuel_layout(lv_obj_t *screen_fuel);

#endif //SCREEN_FUEL_H