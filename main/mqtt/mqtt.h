#ifndef MQTT_H
#define MQTT_H

#include "queue_reader.h"

// Replace with your actual VRM ID
#define VRM_ID "48e7da89d561" 

typedef enum {
// -----Victron-----
    // House Battery
    TOPIC_HOUSE_BATTERY_VOLTAGE,
    TOPIC_HOUSE_BATTERY_CURRENT,
    TOPIC_HOUSE_BATTERY_POWER,
    TOPIC_HOUSE_BATTERY_SOC,
    // Start Battery
    TOPIC_START_BATTERY_VOLTAGE,
    // IP43 charger
    TOPIC_CHARGER_VOLTAGE,
    TOPIC_CHARGER_CURRENT,
    // MPPT1
    TOPIC_MPPT1_DC_VOLTAGE,
    TOPIC_MPPT1_DC_CURRENT,
    TOPIC_MPPT1_PV_VOLTAGE,
    TOPIC_MPPT1_YIELD_TODAY,
    TOPIC_MPPT1_POWER,
    // MPPT2
    TOPIC_MPPT2_DC_VOLTAGE,
    TOPIC_MPPT2_DC_CURRENT,
    TOPIC_MPPT2_PV_VOLTAGE,
    TOPIC_MPPT2_YIELD_TODAY,
    TOPIC_MPPT2_POWER,
    // DC2DC
    TOPIC_DC2DC_VOLTAGE,
    TOPIC_DC2DC_CURRENT,
    TOPIC_DC2DC_POWER,
    //add topics here, keep the last one
    TOPIC_COUNT             // Index 3 (Total count)
} victron_topic_id_t;

// Hardcode the complete strings directly into your registry array
static const char *topic_registry[TOPIC_COUNT] = {
    // House Battery
        [TOPIC_HOUSE_BATTERY_VOLTAGE] = "N/" VRM_ID "/battery/277/Dc/0/Voltage",
        [TOPIC_HOUSE_BATTERY_CURRENT] = "N/" VRM_ID "/battery/277/Dc/0/Current",
        [TOPIC_HOUSE_BATTERY_POWER] = "N/" VRM_ID "/battery/277/Dc/0/Power",
        [TOPIC_HOUSE_BATTERY_SOC] = "N/" VRM_ID "/battery/277/Soc",
    // Start Battery
        [TOPIC_START_BATTERY_VOLTAGE] = "N/" VRM_ID "/battery/277/Dc/1/Voltage",
    // IP43 charger
        [TOPIC_CHARGER_VOLTAGE] = "N/" VRM_ID "/charger/280/Dc/0/Voltage",
        [TOPIC_CHARGER_CURRENT] = "N/" VRM_ID "/charger/280/Dc/0/Current",
    // MPPT1
        [TOPIC_MPPT1_DC_VOLTAGE] = "N/" VRM_ID "/solarcharger/278/Dc/0/Voltage",
        [TOPIC_MPPT1_DC_CURRENT] = "N/" VRM_ID "/solarcharger/278/Dc/0/Current",
        [TOPIC_MPPT1_PV_VOLTAGE] = "N/" VRM_ID "/solarcharger/278/Pv/V",
        [TOPIC_MPPT1_YIELD_TODAY] = "N/" VRM_ID "/solarcharger/278/History/Daily/0/Yield",
        [TOPIC_MPPT1_POWER] = "N/" VRM_ID "/solarcharger/278/Yield/Power",
    // MPPT2
        [TOPIC_MPPT2_DC_VOLTAGE] = "N/" VRM_ID "/solarcharger/290/Dc/0/Voltage",
        [TOPIC_MPPT2_DC_CURRENT] = "N/" VRM_ID "/solarcharger/290/Dc/0/Current",
        [TOPIC_MPPT2_PV_VOLTAGE] = "N/" VRM_ID "/solarcharger/290/Pv/V",
        [TOPIC_MPPT2_YIELD_TODAY] = "N/" VRM_ID "/solarcharger/290/History/Daily/0/Yield",
        [TOPIC_MPPT2_POWER] = "N/" VRM_ID "/solarcharger/290/Yield/Power",
    // OrionXS
        [TOPIC_DC2DC_VOLTAGE] = "N/" VRM_ID "/alternator/279/Dc/0/Voltage",
        [TOPIC_DC2DC_CURRENT] = "N/" VRM_ID "/alternator/279/Dc/0/Current",
        [TOPIC_DC2DC_POWER] = "N/" VRM_ID "/alternator/279/Dc/0/Power"
};

void mqtt_app_start(void);

#endif //MQTT_H