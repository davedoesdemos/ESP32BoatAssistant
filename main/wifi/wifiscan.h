#ifndef WIFISCAN_H
#define WIFISCAN_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "regex.h"

char* wifi_scan(void);

#endif //WIFISCAN_H