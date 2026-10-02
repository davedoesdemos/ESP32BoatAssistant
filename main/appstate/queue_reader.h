#ifndef QUEUE_READER_H
#define QUEUE_READER_H

#include <stdio.h>
#include <math.h>
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
// -----NMEA2k-----
    // Wind
    TOPIC_NMEA_WIND_TRUE_SPEED,
    TOPIC_NMEA_WIND_TRUE_DIRECTION,
    TOPIC_NMEA_WIND_APPARENT_SPEED,
    TOPIC_NMEA_WIND_APPARENT_DIRECTION,
    // Diesel Tank
    TOPIC_NMEA_DIESELTANK_LEVEL_PERCENT,
    TOPIC_NMEA_DIESELTANK_LEVEL_LITRES,
    TOPIC_NMEA_DIESELTANK_CAPACITY,
    // Boat
    TOPIC_NMEA_BOAT_HEADING,
    TOPIC_NMEA_BOAT_COURSE_OVER_GROUND,
    TOPIC_NMEA_BOAT_SPEED_OVER_GROUND,
    TOPIC_NMEA_BOAT_SPEED_THROUGH_WATER,
    TOPIC_NMEA_BOAT_DEPTH,
    TOPIC_NMEA_BOAT_DEPTH_OFFSET,
    TOPIC_NMEA_BOAT_LATITUDE,
    TOPIC_NMEA_BOAT_LONGITUDE,
    // Environment
    TOPIC_NMEA_ENVIRONMENT_TEMPERATURE_INSIDE,
    TOPIC_NMEA_ENVIRONMENT_HUMIDITY_INSIDE,
    TOPIC_NMEA_ENVIRONMENT_TEMPERATURE_OUTSIDE,
    TOPIC_NMEA_ENVIRONMENT_HUMIDITY_OUTSIDE,
// -----Victron-----
    // House Battery
    TOPIC_VICTRON_HOUSE_BATTERY_VOLTAGE,
    TOPIC_VICTRON_HOUSE_BATTERY_CURRENT,
    TOPIC_VICTRON_HOUSE_BATTERY_POWER,
    TOPIC_VICTRON_HOUSE_BATTERY_SOC,
    // Start Battery
    TOPIC_VICTRON_START_BATTERY_VOLTAGE,
    // IP43 charger
    TOPIC_VICTRON_CHARGER_VOLTAGE,
    TOPIC_VICTRON_CHARGER_CURRENT,
    // MPPT1
    TOPIC_VICTRON_MPPT1_DC_VOLTAGE,
    TOPIC_VICTRON_MPPT1_DC_CURRENT,
    TOPIC_VICTRON_MPPT1_PV_VOLTAGE,
    TOPIC_VICTRON_MPPT1_YIELD_TODAY,
    TOPIC_VICTRON_MPPT1_POWER,
    // MPPT2
    TOPIC_VICTRON_MPPT2_DC_VOLTAGE,
    TOPIC_VICTRON_MPPT2_DC_CURRENT,
    TOPIC_VICTRON_MPPT2_PV_VOLTAGE,
    TOPIC_VICTRON_MPPT2_YIELD_TODAY,
    TOPIC_VICTRON_MPPT2_POWER,
    // DC2DC
    TOPIC_VICTRON_DC2DC_VOLTAGE,
    TOPIC_VICTRON_DC2DC_CURRENT,
    TOPIC_VICTRON_DC2DC_POWER,
// -----System-----
    TOPIC_SYSTEM_WIFI_CONNECTED,
    TOPIC_SYSTEM_WIFI_STATUS
} telemetry_id_t;

/*=============================================================================
 * THE QUEUE MESSAGE STRUCT
 *============================================================================*/
typedef struct {
    telemetry_id_t id;  // Which parameter is this? (The Key)
    union {
        float value_float;
        int value_int;
        char value_string[16]; // For pre-formatted strings
    } value;
} telemetry_packet_t;

extern QueueHandle_t msg_queue;

void queue_reader_init(QueueHandle_t hardware_queue);

#endif //QUEUE_READER_H