#ifndef QUEUE_READER_H
#define QUEUE_READER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "lvgl.h"
#include "state.h"

// Unique identifiers for each Victron parameter
typedef enum {
// -----Ruuvi-----
    // Ruuvi Tag 1
    TOPIC_RUUVI_TAG_1_TEMPERATURE,
    TOPIC_RUUVI_TAG_1_HUMIDITY,
    TOPIC_RUUVI_TAG_1_PRESSURE,
    // Ruuvi Tag 2
    TOPIC_RUUVI_TAG_2_TEMPERATURE,
    TOPIC_RUUVI_TAG_2_HUMIDITY,
    TOPIC_RUUVI_TAG_2_PRESSURE,
    // Ruuvi Air 1
    TOPIC_RUUVI_AIR_1_TEMPERATURE,
    TOPIC_RUUVI_AIR_1_HUMIDITY,
    TOPIC_RUUVI_AIR_1_PRESSURE,
    TOPIC_RUUVI_AIR_1_PM2_5, //Particulate Matter (PM) 2.5µm
    TOPIC_RUUVI_AIR_1_CO2, //Carbon Dioxide (CO₂)
    TOPIC_RUUVI_AIR_1_VOC, //VOC Volatile Organic Compounds
    TOPIC_RUUVI_AIR_1_NOX, //NOx Nitrogen Oxides
// -----Victron-----
        // House Battery
    TOPIC_HOUSE_BATTERY_VOLTAGE,
    TOPIC_HOUSE_BATTERY_CURRENT,
    TOPIC_HOUSE_BATTERY_POWER,
    TOPIC_HOUSE_BATTERY_SOC,
       // Start Battery
    TOPIC_START_BATTERY_VOLTAGE,
// -----System-----
    TOPIC_SYSTEM_WIFI_CONNECTED,
    TOPIC_SYSTEM_WIFI_IP_ADDRESS
} telemetry_id_t;

/*=============================================================================
 * THE QUEUE MESSAGE STRUCT
 *============================================================================*/
typedef struct {
    telemetry_id_t id;  // Which parameter is this? (The Key)
    int32_t value;      // The actual numeric value scaled to an integer (The Value)
} telemetry_packet_t;

extern QueueHandle_t msg_queue;

void queue_reader_init(QueueHandle_t hardware_queue);

#endif //QUEUE_READER_H