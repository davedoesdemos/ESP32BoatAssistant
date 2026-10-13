#ifndef MQTT_H
#define MQTT_H

#include "queue_reader.h"

// Replace with your actual VRM ID
#define VRM_ID "48e7da89d561"
#define CERBO_ADDRESS "mqtt://192.168.1.2:1883"

typedef enum {
// -----Victron-----
    // House Battery
    MQTT_TOPIC_HOUSE_BATTERY_VOLTAGE,
    MQTT_TOPIC_HOUSE_BATTERY_CURRENT,
    MQTT_TOPIC_HOUSE_BATTERY_POWER,
    MQTT_TOPIC_HOUSE_BATTERY_SOC,
    // Start Battery
    MQTT_TOPIC_START_BATTERY_VOLTAGE,
    // IP43 charger
    MQTT_TOPIC_CHARGER_VOLTAGE,
    MQTT_TOPIC_CHARGER_CURRENT,
    // MPPT1
    MQTT_TOPIC_MPPT1_DC_VOLTAGE,
    MQTT_TOPIC_MPPT1_DC_CURRENT,
    MQTT_TOPIC_MPPT1_PV_VOLTAGE,
    MQTT_TOPIC_MPPT1_YIELD_TODAY,
    MQTT_TOPIC_MPPT1_POWER,
    // MPPT2
    MQTT_TOPIC_MPPT2_DC_VOLTAGE,
    MQTT_TOPIC_MPPT2_DC_CURRENT,
    MQTT_TOPIC_MPPT2_PV_VOLTAGE,
    MQTT_TOPIC_MPPT2_YIELD_TODAY,
    MQTT_TOPIC_MPPT2_POWER,
    // DC2DC
    MQTT_TOPIC_DC2DC_VOLTAGE,
    MQTT_TOPIC_DC2DC_CURRENT,
    MQTT_TOPIC_DC2DC_POWER,
    // Diesel Tank
    MQTT_TOPIC_DIESELTANK_LEVEL_PERCENT,
    MQTT_TOPIC_DIESELTANK_LEVEL_LITRES,
    MQTT_TOPIC_DIESELTANK_CAPACITY,
    // Inverter
    MQTT_TOPIC_INVERTER_AC_VOLTAGE,
    MQTT_TOPIC_INVERTER_AC_CURRENT,
    MQTT_TOPIC_INVERTER_DV_VOLTAGE,
    //add topics here, keep the last one
    MQTT_TOPIC_COUNT             // (Total count)
} victron_topic_id_t;

// Hardcode the complete strings directly into your registry array
static const char *mqtt_topic_registry[MQTT_TOPIC_COUNT] = {
    // House Battery
        [MQTT_TOPIC_HOUSE_BATTERY_VOLTAGE] = "N/" VRM_ID "/battery/277/Dc/0/Voltage",
        [MQTT_TOPIC_HOUSE_BATTERY_CURRENT] = "N/" VRM_ID "/battery/277/Dc/0/Current",
        [MQTT_TOPIC_HOUSE_BATTERY_POWER] = "N/" VRM_ID "/battery/277/Dc/0/Power",
        [MQTT_TOPIC_HOUSE_BATTERY_SOC] = "N/" VRM_ID "/battery/277/Soc",
    // Start Battery
        [MQTT_TOPIC_START_BATTERY_VOLTAGE] = "N/" VRM_ID "/battery/277/Dc/1/Voltage",
    // IP43 charger
        [MQTT_TOPIC_CHARGER_VOLTAGE] = "N/" VRM_ID "/charger/280/Dc/0/Voltage",
        [MQTT_TOPIC_CHARGER_CURRENT] = "N/" VRM_ID "/charger/280/Dc/0/Current",
    // MPPT1
        [MQTT_TOPIC_MPPT1_DC_VOLTAGE] = "N/" VRM_ID "/solarcharger/278/Dc/0/Voltage",
        [MQTT_TOPIC_MPPT1_DC_CURRENT] = "N/" VRM_ID "/solarcharger/278/Dc/0/Current",
        [MQTT_TOPIC_MPPT1_PV_VOLTAGE] = "N/" VRM_ID "/solarcharger/278/Pv/V",
        [MQTT_TOPIC_MPPT1_YIELD_TODAY] = "N/" VRM_ID "/solarcharger/278/History/Daily/0/Yield",
        [MQTT_TOPIC_MPPT1_POWER] = "N/" VRM_ID "/solarcharger/278/Yield/Power",
    // MPPT2
        [MQTT_TOPIC_MPPT2_DC_VOLTAGE] = "N/" VRM_ID "/solarcharger/290/Dc/0/Voltage",
        [MQTT_TOPIC_MPPT2_DC_CURRENT] = "N/" VRM_ID "/solarcharger/290/Dc/0/Current",
        [MQTT_TOPIC_MPPT2_PV_VOLTAGE] = "N/" VRM_ID "/solarcharger/290/Pv/V",
        [MQTT_TOPIC_MPPT2_YIELD_TODAY] = "N/" VRM_ID "/solarcharger/290/History/Daily/0/Yield",
        [MQTT_TOPIC_MPPT2_POWER] = "N/" VRM_ID "/solarcharger/290/Yield/Power",
    // OrionXS
        [MQTT_TOPIC_DC2DC_VOLTAGE] = "N/" VRM_ID "/alternator/279/Dc/0/Voltage",
        [MQTT_TOPIC_DC2DC_CURRENT] = "N/" VRM_ID "/alternator/279/Dc/0/Current",
        [MQTT_TOPIC_DC2DC_POWER] = "N/" VRM_ID "/alternator/279/Dc/0/Power",
    // Diesel Tank
        [MQTT_TOPIC_DIESELTANK_LEVEL_PERCENT] = "N/" VRM_ID "/tank/20/Level",
        [MQTT_TOPIC_DIESELTANK_LEVEL_LITRES] = "N/" VRM_ID "/tank/20/Remaining",
        [MQTT_TOPIC_DIESELTANK_CAPACITY] = "N/" VRM_ID "/tank/20/Capacity",
    // Inverter
        [MQTT_TOPIC_INVERTER_AC_VOLTAGE] = "N/" VRM_ID "/inverter/278/Ac/Out/L1/V",
        [MQTT_TOPIC_INVERTER_AC_CURRENT] = "N/" VRM_ID "/inverter/278/Ac/Out/L1/I",
        [MQTT_TOPIC_INVERTER_DV_VOLTAGE] = "N/" VRM_ID "/inverter/278/Dc/0/Voltage"

};

void mqtt_app_start(void);

#endif //MQTT_H