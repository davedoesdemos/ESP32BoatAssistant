#ifndef WIFISTATION_H
#define WIFISTATION_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "gui.h"

#include "lwip/err.h"
#include "lwip/sys.h"

// Create a structure to pass strings safely into a task
typedef struct {
    char ssid[32];
    char password[64];
} wifi_credentials_t;

void wifi_global_init(void);
void wifi_task(void *pvParameters);
void wifi_init_sta(const char * ssid, const char * password);

#endif //WIFISTATION_H