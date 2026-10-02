#ifndef STATE_H
#define STATE_H

#include <lvgl.h>
#include <stdbool.h>
#include "stdio.h"
#include <esp_log.h>

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
    extern lv_subject_t * const state_ruuvi_air_1_temperature_text;
    extern lv_subject_t * const state_ruuvi_air_1_humidity;
    extern lv_subject_t * const state_ruuvi_air_1_humidity_text;
    extern lv_subject_t * const state_ruuvi_air_1_pressure;
    extern lv_subject_t * const state_ruuvi_air_1_pressure_text;
    extern lv_subject_t * const state_ruuvi_air_1_pm2_5; //Particulate Matter (PM) 2.5µm
    extern lv_subject_t * const state_ruuvi_air_1_pm2_5_text;
    extern lv_subject_t * const state_ruuvi_air_1_co2; //Carbon Dioxide (CO₂)
    extern lv_subject_t * const state_ruuvi_air_1_co2_text;
    extern lv_subject_t * const state_ruuvi_air_1_voc; //VOC Volatile Organic Compounds
    extern lv_subject_t * const state_ruuvi_air_1_voc_text;
    extern lv_subject_t * const state_ruuvi_air_1_nox; //NOx Nitrogen Oxides
    extern lv_subject_t * const state_ruuvi_air_1_nox_text;

// -----NMEA2k-----
    // Wind
    extern lv_subject_t * const state_nmea_wind_true_speed;
    extern lv_subject_t * const state_nmea_wind_true_speed_text;
    extern lv_subject_t * const state_nmea_wind_true_direction;
    extern lv_subject_t * const state_nmea_wind_true_direction_text;
    extern lv_subject_t * const state_nmea_wind_apparent_speed;
    extern lv_subject_t * const state_nmea_wind_apparent_speed_text;
    extern lv_subject_t * const state_nmea_wind_apparent_direction;
    extern lv_subject_t * const state_nmea_wind_apparent_direction_text;
    // Boat
    extern lv_subject_t * const state_nmea_boat_heading;
    extern lv_subject_t * const state_nmea_boat_heading_text;
    extern lv_subject_t * const state_nmea_boat_course_over_ground;
    extern lv_subject_t * const state_nmea_boat_course_over_ground_text;
    extern lv_subject_t * const state_nmea_boat_speed_over_ground;
    extern lv_subject_t * const state_nmea_boat_speed_over_ground_text;
    extern lv_subject_t * const state_nmea_boat_speed_through_water;
    extern lv_subject_t * const state_nmea_boat_speed_through_water_text;
    extern lv_subject_t * const state_nmea_boat_depth;
    extern lv_subject_t * const state_nmea_boat_depth_text;
    extern lv_subject_t * const state_nmea_boat_depth_offset;
    extern lv_subject_t * const state_nmea_boat_depth_offset_text;
    extern lv_subject_t * const state_nmea_boat_latitude;
    extern lv_subject_t * const state_nmea_boat_latitude_text;
    extern lv_subject_t * const state_nmea_boat_longitude;
    extern lv_subject_t * const state_nmea_boat_longitude_text;
    // Environment
    extern lv_subject_t * const state_nmea_environment_temperature_inside;
    extern lv_subject_t * const state_nmea_environment_temperature_inside_text;
    extern lv_subject_t * const state_nmea_environment_humidity_inside;
    extern lv_subject_t * const state_nmea_environment_humidity_inside_text;
    extern lv_subject_t * const state_nmea_environment_temperature_outside;
    extern lv_subject_t * const state_nmea_environment_temperature_outside_text;
    extern lv_subject_t * const state_nmea_environment_humidity_outside;
    extern lv_subject_t * const state_nmea_environment_humidity_outside_text;
// -----Victron-----
    // House Battery
    extern lv_subject_t * const state_victron_house_battery_voltage;
    extern lv_subject_t * const state_victron_house_battery_voltage_text;
    extern lv_subject_t * const state_victron_house_battery_current;
    extern lv_subject_t * const state_victron_house_battery_current_text;
    extern lv_subject_t * const state_victron_house_battery_power;
    extern lv_subject_t * const state_victron_house_battery_power_text;
    extern lv_subject_t * const state_victron_house_battery_soc;
    extern lv_subject_t * const state_victron_house_battery_soc_text;
    // Start Battery
    extern lv_subject_t * const state_victron_start_battery_voltage;
    extern lv_subject_t * const state_victron_start_battery_voltage_text;
    // IP43 Charger
    extern lv_subject_t * const state_victron_charger_voltage;
    extern lv_subject_t * const state_victron_charger_voltage_text;
    extern lv_subject_t * const state_victron_charger_current;
    extern lv_subject_t * const state_victron_charger_current_text;
    extern lv_subject_t * const state_victron_charger_power;
    extern lv_subject_t * const state_victron_charger_power_text;
    // MPPT 1
    extern lv_subject_t * const state_victron_mppt1_dc_voltage;
    extern lv_subject_t * const state_victron_mppt1_dc_voltage_text;
    extern lv_subject_t * const state_victron_mppt1_dc_current;
    extern lv_subject_t * const state_victron_mppt1_dc_current_text;
    extern lv_subject_t * const state_victron_mppt1_pv_voltage;
    extern lv_subject_t * const state_victron_mppt1_pv_voltage_text;
    extern lv_subject_t * const state_victron_mppt1_yield_today;
    extern lv_subject_t * const state_victron_mppt1_yield_today_text;
    extern lv_subject_t * const state_victron_mppt1_power;
    extern lv_subject_t * const state_victron_mppt1_power_text;
    // MPPT 2
    extern lv_subject_t * const state_victron_mppt2_dc_voltage;
    extern lv_subject_t * const state_victron_mppt2_dc_voltage_text;
    extern lv_subject_t * const state_victron_mppt2_dc_current;
    extern lv_subject_t * const state_victron_mppt2_dc_current_text;
    extern lv_subject_t * const state_victron_mppt2_pv_voltage;
    extern lv_subject_t * const state_victron_mppt2_pv_voltage_text;
    extern lv_subject_t * const state_victron_mppt2_yield_today;
    extern lv_subject_t * const state_victron_mppt2_yield_today_text;
    extern lv_subject_t * const state_victron_mppt2_power;
    extern lv_subject_t * const state_victron_mppt2_power_text;
    // DC2DC
    extern lv_subject_t * const state_victron_dc2dc_voltage;
    extern lv_subject_t * const state_victron_dc2dc_voltage_text;
    extern lv_subject_t * const state_victron_dc2dc_current;
    extern lv_subject_t * const state_victron_dc2dc_current_text;
    extern lv_subject_t * const state_victron_dc2dc_power;
    extern lv_subject_t * const state_victron_dc2dc_power_text;
    // Diesel Tank
    extern lv_subject_t * const state_victron_dieseltank_level_percent;
    extern lv_subject_t * const state_victron_dieseltank_level_percent_text;
    extern lv_subject_t * const state_victron_dieseltank_level_litres;
    extern lv_subject_t * const state_victron_dieseltank_level_litres_text;
    extern lv_subject_t * const state_victron_dieseltank_capacity;
    extern lv_subject_t * const state_victron_dieseltank_capacity_text;
    extern lv_subject_t * const state_victron_dieseltank_remaining_hours;
    extern lv_subject_t * const state_victron_dieseltank_remaining_hours_text;
    extern lv_subject_t * const state_victron_dieseltank_remaining_miles;
    extern lv_subject_t * const state_victron_dieseltank_remaining_miles_text;
// -----System-----
    extern lv_subject_t * const state_system_wifi_connected;
    extern lv_subject_t * const state_system_wifi_status_text;

void state_init(void);

// -----Ruuvi-----
    // Ruuvi Tag 1
    void state_set_ruuvi_tag_1_temperature(int32_t value);
    void state_set_ruuvi_tag_1_humidity(int32_t value);
    void state_set_ruuvi_tag_1_pressure(int32_t value);
    // Ruuvi Tag 2
    void state_set_ruuvi_tag_2_temperature(int32_t value);
    void state_set_ruuvi_tag_2_humidity(int32_t value);
    void state_set_ruuvi_tag_2_pressure(int32_t value);
    // Ruuvi Air 1
    void state_set_ruuvi_air_1_temperature(int32_t value);
    void state_set_ruuvi_air_1_humidity(int32_t value);
    void state_set_ruuvi_air_1_pressure(int32_t value);
    void state_set_ruuvi_air_1_pm2_5(int32_t value);
    void state_set_ruuvi_air_1_co2(int32_t value);
    void state_set_ruuvi_air_1_voc(int32_t value);
    void state_set_ruuvi_air_1_nox(int32_t value);
// -----NMEA2K-----
    // Wind
    void state_set_nmea_wind_true_speed(int32_t value);
    void state_set_nmea_wind_true_direction(int32_t value);
    void state_set_nmea_wind_apparent_speed(int32_t value);
    void state_set_nmea_wind_apparent_direction(int32_t value);
    // Boat
    void state_set_nmea_boat_heading(int32_t value);
    void state_set_nmea_boat_course_over_ground(int32_t value);
    void state_set_nmea_boat_speed_over_ground(int32_t value);
    void state_set_nmea_boat_speed_through_water(int32_t value);
    void state_set_nmea_boat_depth(int32_t value);
    void state_set_nmea_boat_depth_offset(int32_t value);
    void state_set_nmea_boat_latitude(int32_t value);
    void state_set_nmea_boat_longitude(int32_t value);
    // Environment
    void state_set_nmea_environment_temperature_inside(int32_t value);
    void state_set_nmea_environment_humidity_inside(int32_t value);
    void state_set_nmea_environment_temperature_outside(int32_t value);
    void state_set_nmea_environment_humidity_outside(int32_t value);
// -----Victron-----
    // House Battery
    void state_set_victron_house_battery_voltage(float value);
    void state_set_victron_house_battery_current(float value);
    void state_set_victron_house_battery_power(float value);
    void state_set_victron_house_battery_soc(float value);
    // Start Battery
    void state_set_victron_start_battery_voltage(float value);
    // IP43 Charger
    void state_set_victron_charger_voltage(float value);
    void state_set_victron_charger_current(float value);
    // MPPT 1
    void state_set_victron_mppt1_dc_voltage(float value);
    void state_set_victron_mppt1_dc_current(float value);
    void state_set_victron_mppt1_pv_voltage(float value);
    void state_set_victron_mppt1_yield_today(int value);
    void state_set_victron_mppt1_power(float value);
    // MPPT 1
    void state_set_victron_mppt2_dc_voltage(float value);
    void state_set_victron_mppt2_dc_current(float value);
    void state_set_victron_mppt2_pv_voltage(float value);
    void state_set_victron_mppt2_yield_today(int value);
    void state_set_victron_mppt2_power(float value);
    // DC2DC
    void state_set_victron_dc2dc_voltage(float value);
    void state_set_victron_dc2dc_current(float value);
    void state_set_victron_dc2dc_power(float value);
    // Diesel Tank
    void state_set_victron_dieseltank_level_percent(int32_t value);
    void state_set_victron_dieseltank_level_litres(int32_t value);
    void state_set_victron_dieseltank_capacity(int32_t value);
// -----System-----
void state_set_system_wifi_status_text(const char * status);

#endif //STATE_H