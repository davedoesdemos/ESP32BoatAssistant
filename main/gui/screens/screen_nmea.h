#ifndef SCREEN_NMEA_H
#define SCREEN_NMEA_H

#include <stdio.h>
//#include "freertos/queue.h"
#include "lvgl.h"
#include "nmea2k.h"
#include "display.h"

void update_nmea();
void screen_nmea_layout(lv_obj_t *screen_nmea);

#endif //SCREEN_NMEA_H