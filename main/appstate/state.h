#ifndef STATE_H
#define STATE_H

#include <lvgl.h>
#include <stdbool.h>
#include "stdio.h"

// Subject pointers for binding
// -----Ruuvi-----
    // Ruuvi Tag 1
    extern lv_subject_t * const state_ruuvi_tag_1_temperature;
    extern lv_subject_t * const state_ruuvi_tag_1_temperature_text;
    extern lv_subject_t * const state_ruuvi_tag_1_humidity;
    extern lv_subject_t * const state_ruuvi_tag_1_humidity_text;
    extern lv_subject_t * const state_ruuvi_tag_1_pressure;
    extern lv_subject_t * const state_ruuvi_tag_1_pressure_text;
    // Ruuvi Tag 2
    extern lv_subject_t * const state_ruuvi_tag_2_temperature;
    extern lv_subject_t * const state_ruuvi_tag_2_temperature_text;
    extern lv_subject_t * const state_ruuvi_tag_2_humidity;
    extern lv_subject_t * const state_ruuvi_tag_2_humidity_text;
    extern lv_subject_t * const state_ruuvi_tag_2_pressure;
    extern lv_subject_t * const state_ruuvi_tag_2_pressure_text;
    // Ruuvi Air 1
    extern lv_subject_t * const state_ruuvi_air_1_temperature;
    extern lv_subject_t * const state_ruuvi_air_1_humidity;
    extern lv_subject_t * const state_ruuvi_air_1_pressure;
    extern lv_subject_t * const state_ruuvi_air_1_pm2_5; //Particulate Matter (PM) 2.5µm
    extern lv_subject_t * const state_ruuvi_air_1_co2; //Carbon Dioxide (CO₂)
    extern lv_subject_t * const state_ruuvi_air_1_voc; //VOC Volatile Organic Compounds
    extern lv_subject_t * const state_ruuvi_air_1_nox; //NOx Nitrogen Oxides
// -----NMEA2k-----
    // Wind
    extern lv_subject_t * const state_nmea_wind_true_speed;
    extern lv_subject_t * const state_nmea_wind_true_direction;
    extern lv_subject_t * const state_nmea_wind_apparent_speed;
    extern lv_subject_t * const state_nmea_wind_apparent_direction;
    // Diesel Tank
    extern lv_subject_t * const state_nmea_dieseltank_level_percent;
    extern lv_subject_t * const state_nmea_dieseltank_level_litres;
    extern lv_subject_t * const state_nmea_dieseltank_capacity;
    // Boat
    extern lv_subject_t * const state_nmea_boat_heading;
    extern lv_subject_t * const state_nmea_boat_course_over_ground;
    extern lv_subject_t * const state_nmea_boat_speed_over_ground;
    extern lv_subject_t * const state_nmea_boat_speed_through_water;
    extern lv_subject_t * const state_nmea_boat_depth;
    extern lv_subject_t * const state_nmea_boat_depth_offset;
    extern lv_subject_t * const state_nmea_boat_latitude;
    extern lv_subject_t * const state_nmea_boat_longitude;
// -----Victron-----
    // House Battery
    extern lv_subject_t * const state_house_battery_voltage;
    extern lv_subject_t * const state_house_battery_current;
    extern lv_subject_t * const state_house_battery_power;
    extern lv_subject_t * const state_house_battery_soc;
    // Start Battery
    extern lv_subject_t * const state_start_battery_voltage;
    // IP43 Charger
    extern lv_subject_t * const state_charger_voltage;
    extern lv_subject_t * const state_charger_current;
    // MPPT 1
    extern lv_subject_t * const state_mppt1_dc_voltage;
    extern lv_subject_t * const state_mppt1_dc_current;
    extern lv_subject_t * const state_mppt1_pv_voltage;
    extern lv_subject_t * const state_mppt1_yield_today;
    // MPPT 1
    extern lv_subject_t * const state_mppt2_dc_voltage;
    extern lv_subject_t * const state_mppt2_dc_current;
    extern lv_subject_t * const state_mppt2_pv_voltage;
    extern lv_subject_t * const state_mppt2_yield_today;
    // DC2DC
    extern lv_subject_t * const state_dc2dc_voltage;
    extern lv_subject_t * const state_dc2dc_current;
    extern lv_subject_t * const state_dc2dc_power;

// -----System-----
    extern lv_subject_t * const state_system_wifi_connected;
    extern lv_subject_t * const state_system_wifi_status_text;


/* 2. PUBLIC STATE MANAGEMENT API
 *============================================================================*/

/*
 * @brief Initializes the single global state structure and all internal 
 *        LVGL subjects with their safe default startup values.
 * @note  Call this in main.c BEFORE creating queues, tasks, or UI screens.
 */
void state_init(void);

/**
 * @brief Thread-safe setters to push data into the state engine.
 *        These will be called directly by your queue_reader.c module.
 * @{
 */
// -----Ruuvi-----
void state_set_ruuvi_tag_1_temperature(int32_t temperature);
void state_set_ruuvi_tag_1_humidity(int32_t percentage);
void state_set_ruuvi_tag_1_pressure(int32_t hpa);
void state_set_ruuvi_tag_2_temperature(int32_t temperature);
void state_set_ruuvi_tag_2_humidity(int32_t percentage);
void state_set_ruuvi_tag_2_pressure(int32_t hpa);
// -----System-----
void state_set_system_wifi_status_text(const char * status);

#endif //STATE_H