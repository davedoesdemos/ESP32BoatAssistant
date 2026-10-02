#include "state.h"

// System State Subjects
typedef struct {
    // -----Ruuvi-----
    struct {
        // Ruuvi Tag 1
        struct {
            lv_subject_t temperature;
            lv_subject_t temperature_text;
            lv_subject_t humidity;
            lv_subject_t humidity_text;
            lv_subject_t pressure;
            lv_subject_t pressure_text;
        } tag_1;
        // Ruuvi Tag 2
        struct {
            lv_subject_t temperature;
            lv_subject_t temperature_text;
            lv_subject_t humidity;
            lv_subject_t humidity_text;
            lv_subject_t pressure;
            lv_subject_t pressure_text;
        } tag_2;
        // Ruuvi Air 1
        struct {
            lv_subject_t temperature;
            lv_subject_t temperature_text;
            lv_subject_t humidity;
            lv_subject_t humidity_text;
            lv_subject_t pressure;
            lv_subject_t pressure_text;
            lv_subject_t pm2_5; //Particulate Matter (PM) 2.5µm
            lv_subject_t pm2_5_text;
            lv_subject_t co2; //Carbon Dioxide (CO₂)
            lv_subject_t co2_text;
            lv_subject_t voc; //VOC Volatile Organic Compounds
            lv_subject_t voc_text;
            lv_subject_t nox; //NOx Nitrogen Oxides
            lv_subject_t nox_text;
        } air_1;
    } ruuvi;
    // -----NMEA2k-----
    struct {
        // Wind
        struct {
            lv_subject_t true_speed;
            lv_subject_t true_speed_text;
            lv_subject_t true_direction;
            lv_subject_t true_direction_text;
            lv_subject_t apparent_speed;
            lv_subject_t apparent_speed_text;
            lv_subject_t apparent_direction;
            lv_subject_t apparent_direction_text;
        } wind;
        // Diesel Tank
        struct {
            lv_subject_t level_percent;
            lv_subject_t level_percent_text;
            lv_subject_t level_litres;
            lv_subject_t level_litres_text;
            lv_subject_t capacity;
            lv_subject_t capacity_text;
            lv_subject_t remaining_hours;
            lv_subject_t remaining_hours_text;
            lv_subject_t remaining_miles;
            lv_subject_t remaining_miles_text;
        } dieseltank;
        // Boat
        struct {
            lv_subject_t heading;
            lv_subject_t heading_text;
            lv_subject_t course_over_ground;
            lv_subject_t course_over_ground_text;
            lv_subject_t speed_over_ground;
            lv_subject_t speed_over_ground_text;
            lv_subject_t speed_through_water;
            lv_subject_t speed_through_water_text;
            lv_subject_t depth;
            lv_subject_t depth_text;
            lv_subject_t depth_offset;
            lv_subject_t depth_offset_text;
            lv_subject_t latitude;
            lv_subject_t latitude_text;
            lv_subject_t longitude;
            lv_subject_t longitude_text;
        } boat;
        // Environment
        struct {
            lv_subject_t temperature_inside;
            lv_subject_t temperature_inside_text;
            lv_subject_t humidity_inside;
            lv_subject_t humidity_inside_text;
            lv_subject_t temperature_outside;
            lv_subject_t temperature_outside_text;
            lv_subject_t humidity_outside;
            lv_subject_t humidity_outside_text;
        } environment;
    } nmea;
    // -----Victron-----
    struct {
        // House Battery
        struct {
            lv_subject_t voltage;
            lv_subject_t voltage_text;
            lv_subject_t current;
            lv_subject_t current_text;
            lv_subject_t power;
            lv_subject_t power_text;
            lv_subject_t soc;
            lv_subject_t soc_text;
        } house_battery;
        // Start Battery
        struct {
            lv_subject_t voltage;
            lv_subject_t voltage_text;
        } start_battery;
        // IP43 charger
        struct {
            lv_subject_t voltage;
            lv_subject_t voltage_text;
            lv_subject_t current;
            lv_subject_t current_text;
            lv_subject_t power;
            lv_subject_t power_text;
        } charger;
        // MPPT1
        struct {
            lv_subject_t dc_voltage;
            lv_subject_t dc_voltage_text;
            lv_subject_t dc_current;
            lv_subject_t dc_current_text;
            lv_subject_t pv_voltage;
            lv_subject_t pv_voltage_text;
            lv_subject_t yield_today;
            lv_subject_t yield_today_text;
            lv_subject_t power;
            lv_subject_t power_text;
        } mppt1;
        // MPPT2
        struct {
            lv_subject_t dc_voltage;
            lv_subject_t dc_voltage_text;
            lv_subject_t dc_current;
            lv_subject_t dc_current_text;
            lv_subject_t pv_voltage;
            lv_subject_t pv_voltage_text;
            lv_subject_t yield_today;
            lv_subject_t yield_today_text;
            lv_subject_t power;
            lv_subject_t power_text;
        } mppt2;
        // DC2DC
        struct {
            lv_subject_t voltage;
            lv_subject_t voltage_text;
            lv_subject_t current;
            lv_subject_t current_text;
            lv_subject_t power;
            lv_subject_t power_text;
        } dc2dc;
    } victron;
    // -----System-----
    struct {
        lv_subject_t wifi_connected;
        lv_subject_t wifi_status_text;
    } system;
} system_state_t;

static system_state_t system_state;

// Link the externals to the state struct as consts so they can't be changed
// -----Ruuvi-----
    // Ruuvi Tag 1
    lv_subject_t * const state_ruuvi_tag_1_temperature = &system_state.ruuvi.tag_1.temperature;
    lv_subject_t * const state_ruuvi_tag_1_temperature_text = &system_state.ruuvi.tag_1.temperature_text;
    lv_subject_t * const state_ruuvi_tag_1_humidity = &system_state.ruuvi.tag_1.humidity;
    lv_subject_t * const state_ruuvi_tag_1_humidity_text = &system_state.ruuvi.tag_1.humidity_text;
    lv_subject_t * const state_ruuvi_tag_1_pressure = &system_state.ruuvi.tag_1.pressure;
    lv_subject_t * const state_ruuvi_tag_1_pressure_text = &system_state.ruuvi.tag_1.pressure_text;
    // Ruuvi Tag 2
    lv_subject_t * const state_ruuvi_tag_2_temperature = &system_state.ruuvi.tag_2.temperature;
    lv_subject_t * const state_ruuvi_tag_2_temperature_text = &system_state.ruuvi.tag_2.temperature_text;
    lv_subject_t * const state_ruuvi_tag_2_humidity = &system_state.ruuvi.tag_2.humidity;
    lv_subject_t * const state_ruuvi_tag_2_humidity_text = &system_state.ruuvi.tag_2.humidity_text;
    lv_subject_t * const state_ruuvi_tag_2_pressure = &system_state.ruuvi.tag_2.pressure;
    lv_subject_t * const state_ruuvi_tag_2_pressure_text = &system_state.ruuvi.tag_2.pressure_text;
    // Ruuvi Air 1
    lv_subject_t * const state_ruuvi_air_1_temperature = &system_state.ruuvi.air_1.temperature;
    lv_subject_t * const state_ruuvi_air_1_temperature_text = &system_state.ruuvi.air_1.temperature_text;
    lv_subject_t * const state_ruuvi_air_1_humidity = &system_state.ruuvi.air_1.humidity;
    lv_subject_t * const state_ruuvi_air_1_humidity_text = &system_state.ruuvi.air_1.humidity_text;
    lv_subject_t * const state_ruuvi_air_1__pressure = &system_state.ruuvi.air_1.pressure;
    lv_subject_t * const state_ruuvi_air_1__pressure_text = &system_state.ruuvi.air_1.pressure_text;
    lv_subject_t * const state_ruuvi_air_1_pm2_5 = &system_state.ruuvi.air_1.pm2_5;
    lv_subject_t * const state_ruuvi_air_1_pm2_5_text = &system_state.ruuvi.air_1.pm2_5_text;
    lv_subject_t * const state_ruuvi_air_1_co2 = &system_state.ruuvi.air_1.co2;
    lv_subject_t * const state_ruuvi_air_1_co2_text = &system_state.ruuvi.air_1.co2_text;
    lv_subject_t * const state_ruuvi_air_1_voc = &system_state.ruuvi.air_1.voc;
    lv_subject_t * const state_ruuvi_air_1_voc_text = &system_state.ruuvi.air_1.voc_text;
    lv_subject_t * const state_ruuvi_air_1_nox = &system_state.ruuvi.air_1.nox;
    lv_subject_t * const state_ruuvi_air_1_nox_text = &system_state.ruuvi.air_1.nox_text;
// -----NMEA2k-----
    // Wind
    lv_subject_t * const state_nmea_wind_true_speed = &system_state.nmea.wind.true_speed;
    lv_subject_t * const state_nmea_wind_true_speed_text = &system_state.nmea.wind.true_speed_text;
    lv_subject_t * const state_nmea_wind_true_direction = &system_state.nmea.wind.true_direction;
    lv_subject_t * const state_nmea_wind_true_direction_text = &system_state.nmea.wind.true_direction_text;
    lv_subject_t * const state_nmea_wind_apparent_speed = &system_state.nmea.wind.apparent_speed;
    lv_subject_t * const state_nmea_wind_apparent_speed_text = &system_state.nmea.wind.apparent_speed_text;
    lv_subject_t * const state_nmea_wind_apparent_direction = &system_state.nmea.wind.apparent_direction;
    lv_subject_t * const state_nmea_wind_apparent_direction_text = &system_state.nmea.wind.apparent_direction_text;
    // Diesel Tank
    lv_subject_t * const state_nmea_dieseltank_level_percent = &system_state.nmea.dieseltank.level_percent;
    lv_subject_t * const state_nmea_dieseltank_level_percent_text = &system_state.nmea.dieseltank.level_percent_text;
    lv_subject_t * const state_nmea_dieseltank_level_litres = &system_state.nmea.dieseltank.level_litres;
    lv_subject_t * const state_nmea_dieseltank_level_litres_text = &system_state.nmea.dieseltank.level_litres_text;
    lv_subject_t * const state_nmea_dieseltank_capacity = &system_state.nmea.dieseltank.capacity;
    lv_subject_t * const state_nmea_dieseltank_capacity_text = &system_state.nmea.dieseltank.capacity_text;
    lv_subject_t * const state_nmea_dieseltank_remaining_hours = &system_state.nmea.dieseltank.remaining_hours;
    lv_subject_t * const state_nmea_dieseltank_remaining_hours_text = &system_state.nmea.dieseltank.remaining_hours_text;
    lv_subject_t * const state_nmea_dieseltank_remaining_miles = &system_state.nmea.dieseltank.remaining_miles;
    lv_subject_t * const state_nmea_dieseltank_remaining_miles_text = &system_state.nmea.dieseltank.remaining_miles_text;
    // Boat
    lv_subject_t * const state_nmea_boat_heading = &system_state.nmea.boat.heading;
    lv_subject_t * const state_nmea_boat_heading_text = &system_state.nmea.boat.heading_text;
    lv_subject_t * const state_nmea_boat_course_over_ground = &system_state.nmea.boat.course_over_ground;
    lv_subject_t * const state_nmea_boat_course_over_ground_text = &system_state.nmea.boat.course_over_ground_text;
    lv_subject_t * const state_nmea_boat_speed_over_ground = &system_state.nmea.boat.speed_over_ground;
    lv_subject_t * const state_nmea_boat_speed_over_ground_text = &system_state.nmea.boat.speed_over_ground_text;
    lv_subject_t * const state_nmea_boat_speed_through_water = &system_state.nmea.boat.speed_through_water;
    lv_subject_t * const state_nmea_boat_speed_through_water_text = &system_state.nmea.boat.speed_through_water_text;
    lv_subject_t * const state_nmea_boat_depth = &system_state.nmea.boat.depth;
    lv_subject_t * const state_nmea_boat_depth_text = &system_state.nmea.boat.depth_text;
    lv_subject_t * const state_nmea_boat_depth_offset = &system_state.nmea.boat.depth_offset;
    lv_subject_t * const state_nmea_boat_depth_offset_text = &system_state.nmea.boat.depth_offset_text;
    lv_subject_t * const state_nmea_boat_latitude = &system_state.nmea.boat.latitude;
    lv_subject_t * const state_nmea_boat_latitude_text = &system_state.nmea.boat.latitude_text;
    lv_subject_t * const state_nmea_boat_longitude = &system_state.nmea.boat.longitude;
    lv_subject_t * const state_nmea_boat_longitude_text = &system_state.nmea.boat.longitude_text;
    // Environment
    lv_subject_t * const state_nmea_environment_temperature_inside = &system_state.nmea.environment.temperature_inside;
    lv_subject_t * const state_nmea_environment_humidity_inside = &system_state.nmea.environment.humidity_inside;
    lv_subject_t * const state_nmea_environment_temperature_outside = &system_state.nmea.environment.temperature_outside;
    lv_subject_t * const state_nmea_environment_humidity_outside = &system_state.nmea.environment.temperature_outside;
    lv_subject_t * const state_nmea_environment_temperature_inside_text = &system_state.nmea.environment.temperature_inside_text;
    lv_subject_t * const state_nmea_environment_humidity_inside_text = &system_state.nmea.environment.humidity_inside_text;
    lv_subject_t * const state_nmea_environment_temperature_outside_text = &system_state.nmea.environment.temperature_outside_text;
    lv_subject_t * const state_nmea_environment_humidity_outside_text = &system_state.nmea.environment.temperature_outside_text;
// -----Victron-----
    // House Battery
    lv_subject_t * const state_victron_house_battery_voltage = &system_state.victron.house_battery.voltage;
    lv_subject_t * const state_victron_house_battery_voltage_text = &system_state.victron.house_battery.voltage_text;
    lv_subject_t * const state_victron_house_battery_current = &system_state.victron.house_battery.current;
    lv_subject_t * const state_victron_house_battery_current_text = &system_state.victron.house_battery.current_text;
    lv_subject_t * const state_victron_house_battery_power = &system_state.victron.house_battery.power;
    lv_subject_t * const state_victron_house_battery_power_text = &system_state.victron.house_battery.power_text;
    lv_subject_t * const state_victron_house_battery_soc = &system_state.victron.house_battery.soc;
    lv_subject_t * const state_victron_house_battery_soc_text = &system_state.victron.house_battery.soc_text;
    // Start Battery
    lv_subject_t * const state_victron_start_battery_voltage = &system_state.victron.start_battery.voltage;
    lv_subject_t * const state_victron_start_battery_voltage_text = &system_state.victron.start_battery.voltage_text;
    // IP43 Charger
    lv_subject_t * const state_victron_charger_voltage = &system_state.victron.charger.voltage;
    lv_subject_t * const state_victron_charger_voltage_text = &system_state.victron.charger.voltage_text;
    lv_subject_t * const state_victron_charger_current = &system_state.victron.charger.current;
    lv_subject_t * const state_victron_charger_current_text = &system_state.victron.charger.current_text;
    lv_subject_t * const state_victron_charger_power = &system_state.victron.charger.power;
    lv_subject_t * const state_victron_charger_power_text = &system_state.victron.charger.power_text;
    // MPPT 1
    lv_subject_t * const state_victron_mppt1_dc_voltage = &system_state.victron.mppt1.dc_voltage;
    lv_subject_t * const state_victron_mppt1_dc_voltage_text = &system_state.victron.mppt1.dc_voltage_text;
    lv_subject_t * const state_victron_mppt1_dc_current = &system_state.victron.mppt1.dc_current;
    lv_subject_t * const state_victron_mppt1_dc_current_text = &system_state.victron.mppt1.dc_current_text;
    lv_subject_t * const state_victron_mppt1_pv_voltage = &system_state.victron.mppt1.pv_voltage;
    lv_subject_t * const state_victron_mppt1_pv_voltage_text = &system_state.victron.mppt1.pv_voltage_text;
    lv_subject_t * const state_victron_mppt1_yield_today = &system_state.victron.mppt1.yield_today;
    lv_subject_t * const state_victron_mppt1_yield_today_text = &system_state.victron.mppt1.yield_today_text;
    lv_subject_t * const state_victron_mppt1_power = &system_state.victron.mppt1.power;
    lv_subject_t * const state_victron_mppt1_power_text = &system_state.victron.mppt1.power_text;
    // MPPT 2
    lv_subject_t * const state_victron_mppt2_dc_voltage = &system_state.victron.mppt2.dc_voltage;
    lv_subject_t * const state_victron_mppt2_dc_voltage_text = &system_state.victron.mppt2.dc_voltage_text;
    lv_subject_t * const state_victron_mppt2_dc_current = &system_state.victron.mppt2.dc_current;
    lv_subject_t * const state_victron_mppt2_dc_current_text = &system_state.victron.mppt2.dc_current_text;
    lv_subject_t * const state_victron_mppt2_pv_voltage = &system_state.victron.mppt2.pv_voltage;
    lv_subject_t * const state_victron_mppt2_pv_voltage_text = &system_state.victron.mppt2.pv_voltage_text;
    lv_subject_t * const state_victron_mppt2_yield_today = &system_state.victron.mppt2.yield_today;
    lv_subject_t * const state_victron_mppt2_yield_today_text = &system_state.victron.mppt2.yield_today_text;
    lv_subject_t * const state_victron_mppt2_power = &system_state.victron.mppt2.power;
    lv_subject_t * const state_victron_mppt2_power_text = &system_state.victron.mppt2.power_text;
    // DC2DC
    lv_subject_t * const state_victron_dc2dc_voltage = &system_state.victron.dc2dc.voltage;
    lv_subject_t * const state_victron_dc2dc_voltage_text = &system_state.victron.dc2dc.voltage_text;
    lv_subject_t * const state_victron_dc2dc_current = &system_state.victron.dc2dc.current;
    lv_subject_t * const state_victron_dc2dc_current_text = &system_state.victron.dc2dc.current_text;
    lv_subject_t * const state_victron_dc2dc_power = &system_state.victron.dc2dc.power;
    lv_subject_t * const state_victron_dc2dc_power_text = &system_state.victron.dc2dc.power_text;
// -----System-----
    lv_subject_t * const state_system_wifi_connected = &system_state.system.wifi_connected;
    lv_subject_t * const state_system_wifi_status_text = &system_state.system.wifi_status_text;

//char buffers for text states
// -----Ruuvi-----
    // -----Ruuvi Tag 1
    static char state_ruuvi_tag_1_temperature_text_buf[16] = "---°C";
    static char state_ruuvi_tag_1_temperature_text_prev_buf[16] = "---°C";
    static char state_ruuvi_tag_1_humidity_text_buf[16] = "---%";
    static char state_ruuvi_tag_1_humidity_text_prev_buf[16] = "---%";
    static char state_ruuvi_tag_1_pressure_text_buf[16] = "---hPa";
    static char state_ruuvi_tag_1_pressure_text_prev_buf[16] = "---hPa";
    // -----Ruuvi Tag 2
    static char state_ruuvi_tag_2_temperature_text_buf[16] = "---°C";
    static char state_ruuvi_tag_2_temperature_text_prev_buf[16] = "---°C";
    static char state_ruuvi_tag_2_humidity_text_buf[16] = "---%";
    static char state_ruuvi_tag_2_humidity_text_prev_buf[16] = "---%";
    static char state_ruuvi_tag_2_pressure_text_buf[16] = "---hPa";
    static char state_ruuvi_tag_2_pressure_text_prev_buf[16] = "---hPa";
    // -----Ruuvi Air 1
    static char state_ruuvi_air_1_temperature_text_buf[16] = "---";
    static char state_ruuvi_air_1_temperature_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_humidity_text_buf[16] = "---";
    static char state_ruuvi_air_1_humidity_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_pressure_text_buf[16] = "---";
    static char state_ruuvi_air_1_pressure_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_pm2_5_text_buf[16] = "---";
    static char state_ruuvi_air_1_pm2_5_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_co2_text_buf[16] = "---";
    static char state_ruuvi_air_1_co2_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_voc_text_buf[16] = "---";
    static char state_ruuvi_air_1_voc_text_prev_buf[16] = "---";
    static char state_ruuvi_air_1_nox_text_buf[16] = "---";
    static char state_ruuvi_air_1_nox_text_prev_buf[16] = "---";
// -----NMEA2k-----
    // Wind
    static char state_nmea_wind_true_speed_text_buf[16] = "---";
    static char state_nmea_wind_true_speed_text_prev_buf[16] = "---";
    static char state_nmea_wind_true_direction_text_buf[16] = "---";
    static char state_nmea_wind_true_direction_text_prev_buf[16] = "---";
    static char state_nmea_wind_apparent_speed_text_buf[16] = "---";
    static char state_nmea_wind_apparent_speed_text_prev_buf[16] = "---";
    static char state_nmea_wind_apparent_direction_text_buf[16] = "---";
    static char state_nmea_wind_apparent_direction_text_prev_buf[16] = "---";
    // Diesel Tank
    static char state_nmea_dieseltank_level_percent_text_buf[16] = "---";
    static char state_nmea_dieseltank_level_percent_text_prev_buf[16] = "---";
    static char state_nmea_dieseltank_level_litres_text_buf[16] = "---";
    static char state_nmea_dieseltank_level_litres_text_prev_buf[16] = "---";
    static char state_nmea_dieseltank_capacity_text_buf[16] = "---";
    static char state_nmea_dieseltank_capacity_text_prev_buf[16] = "---";
    static char state_nmea_dieseltank_remaining_hours_text_buf[16] = "---";
    static char state_nmea_dieseltank_remaining_hours_text_prev_buf[16] = "---";
    static char state_nmea_dieseltank_remaining_miles_text_buf[16] = "---";
    static char state_nmea_dieseltank_remaining_miles_text_prev_buf[16] = "---";
    // Boat
    static char state_nmea_boat_heading_text_buf[16] = "---";
    static char state_nmea_boat_heading_text_prev_buf[16] = "---";
    static char state_nmea_boat_course_over_ground_text_buf[16] = "---";
    static char state_nmea_boat_course_over_ground_text_prev_buf[16] = "---";
    static char state_nmea_boat_speed_over_ground_text_buf[16] = "---";
    static char state_nmea_boat_speed_over_ground_text_prev_buf[16] = "---";
    static char state_nmea_boat_speed_through_water_text_buf[16] = "---";
    static char state_nmea_boat_speed_through_water_text_prev_buf[16] = "---";
    static char state_nmea_boat_depth_text_buf[16] = "---";
    static char state_nmea_boat_depth_text_prev_buf[16] = "---";
    static char state_nmea_boat_depth_offset_text_buf[16] = "---";
    static char state_nmea_boat_depth_offset_text_prev_buf[16] = "---";
    static char state_nmea_boat_latitude_text_buf[16] = "---";
    static char state_nmea_boat_latitude_text_prev_buf[16] = "---";
    static char state_nmea_boat_longitude_text_buf[16] = "---";
    static char state_nmea_boat_longitude_text_prev_buf[16] = "---";
    // Environment
    static char state_nmea_environment_temperature_inside_text_buf[16] = "---";
    static char state_nmea_environment_temperature_inside_text_prev_buf[16] = "---";
    static char state_nmea_environment_humidity_inside_text_buf[16] = "---";
    static char state_nmea_environment_humidity_inside_text_prev_buf[16] = "---";
    static char state_nmea_environment_temperature_outside_text_buf[16] = "---";
    static char state_nmea_environment_temperature_outside_text_prev_buf[16] = "---";
    static char state_nmea_environment_humidity_outside_text_buf[16] = "---";
    static char state_nmea_environment_humidity_outside_text_prev_buf[16] = "---";
// -----Victron-----
    // House Battery
    static char state_victron_house_battery_voltage_text_buf[16] = "---";
    static char state_victron_house_battery_voltage_text_prev_buf[16] = "---";
    static char state_victron_house_battery_current_text_buf[16] = "---";
    static char state_victron_house_battery_current_text_prev_buf[16] = "---";
    static char state_victron_house_battery_power_text_buf[16] = "---";
    static char state_victron_house_battery_power_text_prev_buf[16] = "---";
    static char state_victron_house_battery_soc_text_buf[16] = "---";
    static char state_victron_house_battery_soc_text_prev_buf[16] = "---";
    // Start Battery
    static char state_victron_start_battery_voltage_text_buf[16] = "---";
    static char state_victron_start_battery_voltage_text_prev_buf[16] = "---";
    // IP43 Charger
    static char state_victron_charger_voltage_text_buf[16] = "---";
    static char state_victron_charger_voltage_text_prev_buf[16] = "---";
    static char state_victron_charger_current_text_buf[16] = "---";
    static char state_victron_charger_current_text_prev_buf[16] = "---";
    static char state_victron_charger_power_text_buf[16] = "---";
    static char state_victron_charger_power_text_prev_buf[16] = "---";
    // MPPT 1
    static char state_victron_mppt1_dc_voltage_text_buf[16] = "---";
    static char state_victron_mppt1_dc_voltage_text_prev_buf[16] = "---";
    static char state_victron_mppt1_dc_current_text_buf[16] = "---";
    static char state_victron_mppt1_dc_current_text_prev_buf[16] = "---";
    static char state_victron_mppt1_pv_voltage_text_buf[16] = "---";
    static char state_victron_mppt1_pv_voltage_text_prev_buf[16] = "---";
    static char state_victron_mppt1_yield_today_text_buf[16] = "---";
    static char state_victron_mppt1_yield_today_text_prev_buf[16] = "---";
    static char state_victron_mppt1_power_text_buf[16] = "---";
    static char state_victron_mppt1_power_text_prev_buf[16] = "---";
    // MPPT 2
    static char state_victron_mppt2_dc_voltage_text_buf[16] = "---";
    static char state_victron_mppt2_dc_voltage_text_prev_buf[16] = "---";
    static char state_victron_mppt2_dc_current_text_buf[16] = "---";
    static char state_victron_mppt2_dc_current_text_prev_buf[16] = "---";
    static char state_victron_mppt2_pv_voltage_text_buf[16] = "---";
    static char state_victron_mppt2_pv_voltage_text_prev_buf[16] = "---";
    static char state_victron_mppt2_yield_today_text_buf[16] = "---";
    static char state_victron_mppt2_yield_today_text_prev_buf[16] = "---";
    static char state_victron_mppt2_power_text_buf[16] = "---";
    static char state_victron_mppt2_power_text_prev_buf[16] = "---";
    // DC2DC
    static char state_victron_dc2dc_voltage_text_buf[16] = "---";
    static char state_victron_dc2dc_voltage_text_prev_buf[16] = "---";
    static char state_victron_dc2dc_current_text_buf[16] = "---";
    static char state_victron_dc2dc_current_text_prev_buf[16] = "---";
    static char state_victron_dc2dc_power_text_buf[16] = "---";
    static char state_victron_dc2dc_power_text_prev_buf[16] = "---";
// -----System-----
static char state_system_wifi_status_text_buf[30] = "Not Connected";
static char state_system_wifi_status_text_prev_buf[30] = "Not Connected";

void state_init(void) {
    // Initialize the actual subjects embedded inside the private struct
    // -----Ruuvi-----
        // Ruuvi Tag 1
        lv_subject_init_int(&system_state.ruuvi.tag_1.temperature, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_1.temperature_text, state_ruuvi_tag_1_temperature_text_buf, state_ruuvi_tag_1_temperature_text_prev_buf, sizeof(state_ruuvi_tag_1_temperature_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.tag_1.humidity, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_1.humidity_text, state_ruuvi_tag_1_humidity_text_buf, state_ruuvi_tag_1_humidity_text_prev_buf, sizeof(state_ruuvi_tag_1_humidity_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.tag_1.pressure, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_1.pressure_text, state_ruuvi_tag_1_pressure_text_buf, state_ruuvi_tag_1_pressure_text_prev_buf, sizeof(state_ruuvi_tag_1_pressure_text_buf), "---");
        // Ruuvi Tag 2
        lv_subject_init_int(&system_state.ruuvi.tag_2.temperature, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_2.temperature_text, state_ruuvi_tag_2_temperature_text_buf, state_ruuvi_tag_2_temperature_text_prev_buf, sizeof(state_ruuvi_tag_2_temperature_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.tag_2.humidity, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_2.humidity_text, state_ruuvi_tag_2_humidity_text_buf, state_ruuvi_tag_2_humidity_text_prev_buf, sizeof(state_ruuvi_tag_2_humidity_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.tag_2.pressure, 0);
        lv_subject_init_string(&system_state.ruuvi.tag_2.pressure_text, state_ruuvi_tag_2_pressure_text_buf, state_ruuvi_tag_2_pressure_text_prev_buf, sizeof(state_ruuvi_tag_2_pressure_text_buf), "---");
        // Ruuvi Air 1
        lv_subject_init_int(&system_state.ruuvi.air_1.temperature, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.temperature_text, state_ruuvi_air_1_temperature_text_buf, state_ruuvi_air_1_temperature_text_prev_buf, sizeof(state_ruuvi_air_1_temperature_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.humidity, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.humidity_text, state_ruuvi_air_1_humidity_text_buf, state_ruuvi_air_1_humidity_text_prev_buf, sizeof(state_ruuvi_air_1_humidity_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.pressure, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.pressure_text, state_ruuvi_air_1_pressure_text_buf, state_ruuvi_air_1_pressure_text_prev_buf, sizeof(state_ruuvi_air_1_pressure_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.pm2_5, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.pm2_5_text, state_ruuvi_air_1_pm2_5_text_buf, state_ruuvi_air_1_pm2_5_text_prev_buf, sizeof(state_ruuvi_air_1_pm2_5_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.co2, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.co2_text, state_ruuvi_air_1_co2_text_buf, state_ruuvi_air_1_co2_text_prev_buf, sizeof(state_ruuvi_air_1_co2_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.voc, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.voc_text, state_ruuvi_air_1_voc_text_buf, state_ruuvi_air_1_voc_text_prev_buf, sizeof(state_ruuvi_air_1_voc_text_buf), "---");
        lv_subject_init_int(&system_state.ruuvi.air_1.nox, 0);
        lv_subject_init_string(&system_state.ruuvi.air_1.nox_text, state_ruuvi_air_1_nox_text_buf, state_ruuvi_air_1_nox_text_prev_buf, sizeof(state_ruuvi_air_1_nox_text_buf), "---");
    // -----NMEA2k-----
        // Wind
        lv_subject_init_int(&system_state.nmea.wind.true_speed, 0);
        lv_subject_init_string(&system_state.nmea.wind.true_speed_text, state_nmea_wind_true_speed_text_buf, state_nmea_wind_true_speed_text_prev_buf, sizeof(state_nmea_wind_true_speed_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.wind.true_direction, 0);
        lv_subject_init_string(&system_state.nmea.wind.true_direction_text, state_nmea_wind_true_direction_text_buf, state_nmea_wind_true_direction_text_prev_buf, sizeof(state_nmea_wind_true_direction_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.wind.apparent_speed, 0);
        lv_subject_init_string(&system_state.nmea.wind.apparent_speed_text, state_nmea_wind_apparent_speed_text_buf, state_nmea_wind_apparent_speed_text_prev_buf, sizeof(state_nmea_wind_apparent_speed_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.wind.apparent_direction, 0);
        lv_subject_init_string(&system_state.nmea.wind.apparent_direction_text, state_nmea_wind_apparent_direction_text_buf, state_nmea_wind_apparent_direction_text_prev_buf, sizeof(state_nmea_wind_apparent_direction_text_buf), "---");
        // Diesel Tank
        lv_subject_init_int(&system_state.nmea.dieseltank.level_percent, 0);
        lv_subject_init_string(&system_state.nmea.dieseltank.level_percent_text, state_nmea_dieseltank_level_percent_text_buf, state_nmea_dieseltank_level_percent_text_prev_buf, sizeof(state_nmea_dieseltank_level_percent_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.dieseltank.level_litres, 0);
        lv_subject_init_string(&system_state.nmea.dieseltank.level_litres_text, state_nmea_dieseltank_level_litres_text_buf, state_nmea_dieseltank_level_litres_text_prev_buf, sizeof(state_nmea_dieseltank_level_litres_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.dieseltank.capacity, 0);
        lv_subject_init_string(&system_state.nmea.dieseltank.capacity_text, state_nmea_dieseltank_capacity_text_buf, state_nmea_dieseltank_capacity_text_prev_buf, sizeof(state_nmea_dieseltank_capacity_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.dieseltank.remaining_hours, 0);
        lv_subject_init_string(&system_state.nmea.dieseltank.remaining_hours_text, state_nmea_dieseltank_remaining_hours_text_buf, state_nmea_dieseltank_remaining_hours_text_prev_buf, sizeof(state_nmea_dieseltank_remaining_hours_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.dieseltank.remaining_miles, 0);
        lv_subject_init_string(&system_state.nmea.dieseltank.remaining_miles_text, state_nmea_dieseltank_remaining_miles_text_buf, state_nmea_dieseltank_remaining_miles_text_prev_buf, sizeof(state_nmea_dieseltank_remaining_miles_text_buf), "---");
        // Boat
        lv_subject_init_int(&system_state.nmea.boat.heading, 0);
        lv_subject_init_string(&system_state.nmea.boat.heading_text, state_nmea_boat_heading_text_buf, state_nmea_boat_heading_text_prev_buf, sizeof(state_nmea_boat_heading_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.course_over_ground, 0);
        lv_subject_init_string(&system_state.nmea.boat.course_over_ground_text, state_nmea_boat_course_over_ground_text_buf, state_nmea_boat_course_over_ground_text_prev_buf, sizeof(state_nmea_boat_course_over_ground_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.speed_over_ground, 0);
        lv_subject_init_string(&system_state.nmea.boat.speed_over_ground_text, state_nmea_boat_speed_over_ground_text_buf, state_nmea_boat_speed_over_ground_text_prev_buf, sizeof(state_nmea_boat_speed_over_ground_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.speed_through_water, 0);
        lv_subject_init_string(&system_state.nmea.boat.speed_through_water_text, state_nmea_boat_speed_through_water_text_buf, state_nmea_boat_speed_through_water_text_prev_buf, sizeof(state_nmea_boat_speed_through_water_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.depth, 0);
        lv_subject_init_string(&system_state.nmea.boat.depth_text, state_nmea_boat_depth_text_buf, state_nmea_boat_depth_text_prev_buf, sizeof(state_nmea_boat_depth_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.depth_offset, 0);
        lv_subject_init_string(&system_state.nmea.boat.depth_offset_text, state_nmea_boat_depth_offset_text_buf, state_nmea_boat_depth_offset_text_prev_buf, sizeof(state_nmea_boat_depth_offset_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.latitude, 0);
        lv_subject_init_string(&system_state.nmea.boat.latitude_text, state_nmea_boat_latitude_text_buf, state_nmea_boat_latitude_text_prev_buf, sizeof(state_nmea_boat_latitude_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.boat.longitude, 0);
        lv_subject_init_string(&system_state.nmea.boat.longitude_text, state_nmea_boat_longitude_text_buf, state_nmea_boat_longitude_text_prev_buf, sizeof(state_nmea_boat_longitude_text_buf), "---");
        // Environment
        lv_subject_init_int(&system_state.nmea.environment.temperature_inside, 0);
        lv_subject_init_string(&system_state.nmea.environment.temperature_inside_text, state_nmea_environment_temperature_inside_text_buf, state_nmea_environment_temperature_inside_text_prev_buf, sizeof(state_nmea_environment_temperature_inside_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.environment.humidity_inside, 0);
        lv_subject_init_string(&system_state.nmea.environment.humidity_inside_text, state_nmea_environment_humidity_inside_text_buf, state_nmea_environment_humidity_inside_text_prev_buf, sizeof(state_nmea_environment_humidity_inside_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.environment.temperature_outside, 0);
        lv_subject_init_string(&system_state.nmea.environment.temperature_outside_text, state_nmea_environment_temperature_outside_text_buf, state_nmea_environment_temperature_outside_text_prev_buf, sizeof(state_nmea_environment_temperature_outside_text_buf), "---");
        lv_subject_init_int(&system_state.nmea.environment.humidity_outside, 0);
        lv_subject_init_string(&system_state.nmea.environment.humidity_outside_text, state_nmea_environment_humidity_outside_text_buf, state_nmea_environment_humidity_outside_text_prev_buf, sizeof(state_nmea_environment_humidity_outside_text_buf), "---");
    // -----Victron-----
        // House Battery
        lv_subject_init_int(&system_state.victron.house_battery.voltage, 0);
        lv_subject_init_string(&system_state.victron.house_battery.voltage_text, state_victron_house_battery_voltage_text_buf , state_victron_house_battery_voltage_text_prev_buf, sizeof(state_victron_house_battery_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.house_battery.current, 0);
        lv_subject_init_string(&system_state.victron.house_battery.current_text, state_victron_house_battery_current_text_buf, state_victron_house_battery_current_text_prev_buf, sizeof(state_victron_house_battery_current_text_buf), "---");
        lv_subject_init_int(&system_state.victron.house_battery.power, 0);
        lv_subject_init_string(&system_state.victron.house_battery.power_text, state_victron_house_battery_power_text_buf, state_victron_house_battery_power_text_prev_buf, sizeof(state_victron_house_battery_power_text_buf), "---");
        lv_subject_init_int(&system_state.victron.house_battery.soc,     100);
        lv_subject_init_string(&system_state.victron.house_battery.soc_text, state_victron_house_battery_soc_text_buf, state_victron_house_battery_soc_text_prev_buf, sizeof(state_victron_house_battery_soc_text_buf), "---");
        // Start Battery
        lv_subject_init_int(&system_state.victron.start_battery.voltage,       0);
        lv_subject_init_string(&system_state.victron.start_battery.voltage_text, state_victron_start_battery_voltage_text_buf, state_victron_start_battery_voltage_text_prev_buf, sizeof(state_victron_start_battery_voltage_text_buf), "---");
        // IP43 Charger
        lv_subject_init_int(&system_state.victron.charger.voltage,       0);
        lv_subject_init_string(&system_state.victron.charger.voltage_text, state_victron_charger_voltage_text_buf, state_victron_charger_voltage_text_prev_buf, sizeof(state_victron_charger_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.charger.current,       0);
        lv_subject_init_string(&system_state.victron.charger.current_text, state_victron_charger_current_text_buf, state_victron_charger_current_text_prev_buf, sizeof(state_victron_charger_current_text_buf), "---");
        lv_subject_init_int(&system_state.victron.charger.power,       0);
        lv_subject_init_string(&system_state.victron.charger.power_text, state_victron_charger_power_text_buf, state_victron_charger_power_text_prev_buf, sizeof(state_victron_charger_power_text_buf), "---");
        // MPPT 1
        lv_subject_init_int(&system_state.victron.mppt1.dc_voltage,       0);
        lv_subject_init_string(&system_state.victron.mppt1.dc_voltage_text, state_victron_mppt1_dc_voltage_text_buf, state_victron_mppt1_dc_voltage_text_prev_buf, sizeof(state_victron_mppt1_dc_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt1.dc_current,       0);
        lv_subject_init_string(&system_state.victron.mppt1.dc_current_text, state_victron_mppt1_dc_current_text_buf, state_victron_mppt1_dc_current_text_prev_buf, sizeof(state_victron_mppt1_dc_current_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt1.pv_voltage,       0);
        lv_subject_init_string(&system_state.victron.mppt1.pv_voltage_text, state_victron_mppt1_pv_voltage_text_buf, state_victron_mppt1_pv_voltage_text_prev_buf, sizeof(state_victron_mppt1_pv_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt1.yield_today,       0);
        lv_subject_init_string(&system_state.victron.mppt1.yield_today_text, state_victron_mppt1_yield_today_text_buf, state_victron_mppt1_yield_today_text_prev_buf, sizeof(state_victron_mppt1_yield_today_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt1.power,       0);
        lv_subject_init_string(&system_state.victron.mppt1.power_text, state_victron_mppt1_power_text_buf, state_victron_mppt1_power_text_prev_buf, sizeof(state_victron_mppt1_power_text_buf), "---");
        // MPPT 1
        lv_subject_init_int(&system_state.victron.mppt2.dc_voltage,       0);
        lv_subject_init_string(&system_state.victron.mppt2.dc_voltage_text, state_victron_mppt2_dc_voltage_text_buf, state_victron_mppt2_dc_voltage_text_prev_buf, sizeof(state_victron_mppt2_dc_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt2.dc_current,       0);
        lv_subject_init_string(&system_state.victron.mppt2.dc_current_text, state_victron_mppt2_dc_current_text_buf, state_victron_mppt2_dc_current_text_prev_buf, sizeof(state_victron_mppt2_dc_current_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt2.pv_voltage,       0);
        lv_subject_init_string(&system_state.victron.mppt2.pv_voltage_text, state_victron_mppt2_pv_voltage_text_buf, state_victron_mppt2_pv_voltage_text_prev_buf, sizeof(state_victron_mppt2_pv_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt2.yield_today,       0);
        lv_subject_init_string(&system_state.victron.mppt2.yield_today_text, state_victron_mppt2_yield_today_text_buf, state_victron_mppt2_yield_today_text_prev_buf, sizeof(state_victron_mppt2_yield_today_text_buf), "---");
        lv_subject_init_int(&system_state.victron.mppt2.power,       0);
        lv_subject_init_string(&system_state.victron.mppt2.power_text, state_victron_mppt2_power_text_buf, state_victron_mppt2_power_text_prev_buf, sizeof(state_victron_mppt2_power_text_buf), "---");
        // DC2DC
        lv_subject_init_int(&system_state.victron.dc2dc.voltage,       0);
        lv_subject_init_string(&system_state.victron.dc2dc.voltage_text, state_victron_dc2dc_voltage_text_buf, state_victron_dc2dc_voltage_text_prev_buf, sizeof(state_victron_dc2dc_voltage_text_buf), "---");
        lv_subject_init_int(&system_state.victron.dc2dc.current,       0);
        lv_subject_init_string(&system_state.victron.dc2dc.current_text, state_victron_dc2dc_current_text_buf, state_victron_dc2dc_current_text_prev_buf, sizeof(state_victron_dc2dc_current_text_buf), "---");
        lv_subject_init_int(&system_state.victron.dc2dc.power,       0);
        lv_subject_init_string(&system_state.victron.dc2dc.power_text, state_victron_dc2dc_power_text_buf, state_victron_dc2dc_power_text_prev_buf, sizeof(state_victron_dc2dc_power_text_buf), "---");
// -----System-----
    lv_subject_init_int(&system_state.system.wifi_connected,       0);
    lv_subject_init_string(&system_state.system.wifi_status_text, state_system_wifi_status_text_buf, state_system_wifi_status_text_prev_buf, sizeof(state_system_wifi_status_text_buf), "Not Connected");
}

// -----Ruuvi-----
// Ruuvi Tag 1
void state_set_ruuvi_tag_1_temperature(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_1.temperature, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_1.temperature_text, temp);
}
void state_set_ruuvi_tag_1_humidity(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_1.humidity, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_1.humidity_text, temp);
}
void state_set_ruuvi_tag_1_pressure(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_1.pressure, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dhPa", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_1.pressure_text, temp);
}
// Ruuvi Tag 2
void state_set_ruuvi_tag_2_temperature(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_2.temperature, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_2.temperature_text, temp);
}
void state_set_ruuvi_tag_2_humidity(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_2.humidity, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_2.humidity_text, temp);
}
void state_set_ruuvi_tag_2_pressure(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.tag_2.pressure, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dhPa", (int)value);
    lv_subject_set_string(&system_state.ruuvi.tag_2.pressure_text, temp);
}
// Ruuvi Air 1
void state_set_ruuvi_air_1_temperature(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.temperature, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.temperature_text, temp);
}
void state_set_ruuvi_air_1_humidity(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.humidity, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.humidity_text, temp);
}
void state_set_ruuvi_air_1_pressure(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.pressure, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dhPa", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.pressure_text, temp);
}
void state_set_ruuvi_air_1_pm2_5(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.pm2_5, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.pm2_5_text, temp);
}
void state_set_ruuvi_air_1_co2(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.co2, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.co2_text, temp);
}
void state_set_ruuvi_air_1_voc(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.voc, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.voc_text, temp);
}
void state_set_ruuvi_air_1_nox(int32_t value){
    lv_subject_set_int(&system_state.ruuvi.air_1.nox, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.ruuvi.air_1.nox_text, temp);
}
// -----NMEA2K-----
// Wind
void state_set_nmea_wind_true_speed(int32_t value) {
    lv_subject_set_int(&system_state.nmea.wind.true_speed, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkt", (int)value);
    lv_subject_set_string(&system_state.nmea.wind.true_speed_text, temp);
}
void state_set_nmea_wind_true_direction(int32_t value) {
    lv_subject_set_int(&system_state.nmea.wind.true_direction, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°", (int)value);
    lv_subject_set_string(&system_state.nmea.wind.true_direction_text, temp);
}
void state_set_nmea_wind_apparent_speed(int32_t value) {
    lv_subject_set_int(&system_state.nmea.wind.apparent_speed, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkt", (int)value);
    lv_subject_set_string(&system_state.nmea.wind.apparent_speed_text, temp);
}
void state_set_nmea_wind_apparent_direction(int32_t value) {
    lv_subject_set_int(&system_state.nmea.wind.apparent_direction, (int)value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°", (int)value);
    lv_subject_set_string(&system_state.nmea.wind.apparent_direction_text, temp);
}
// Diesel Tank
void state_set_nmea_dieseltank_level_percent(int32_t value) {
    lv_subject_set_int(&system_state.nmea.dieseltank.level_percent, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.nmea.dieseltank.level_percent_text, temp);
}
void state_set_nmea_dieseltank_level_litres(int32_t value) {
    lv_subject_set_int(&system_state.nmea.dieseltank.level_litres, value);
    char temp[16];
    int fuel_time = value / 2;
    lv_subject_set_int(&system_state.nmea.dieseltank.remaining_hours, fuel_time);
    int fuel_miles = fuel_time * 5;
    lv_subject_set_int(&system_state.nmea.dieseltank.remaining_miles, fuel_miles);
    snprintf(temp, sizeof(temp), "%dl", (int)value);
    lv_subject_set_string(&system_state.nmea.dieseltank.level_litres_text, temp);
    snprintf(temp, sizeof(temp), "%dhrs", fuel_time);
    lv_subject_set_string(&system_state.nmea.dieseltank.remaining_hours_text, temp);
    snprintf(temp, sizeof(temp), "%dNM", fuel_miles);
    lv_subject_set_string(&system_state.nmea.dieseltank.remaining_miles_text, temp);
}
void state_set_nmea_dieseltank_capacity(int32_t value) {
    lv_subject_set_int(&system_state.nmea.dieseltank.capacity, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dl", (int)value);
    lv_subject_set_string(&system_state.nmea.dieseltank.capacity_text, temp);
}
// Boat
void state_set_nmea_boat_heading(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.heading, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.heading_text, temp);
}
void state_set_nmea_boat_course_over_ground(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.course_over_ground, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.course_over_ground_text, temp);
}
void state_set_nmea_boat_speed_over_ground(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.speed_over_ground, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkt", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.speed_over_ground_text, temp);
}
void state_set_nmea_boat_speed_through_water(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.speed_through_water, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkt", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.speed_through_water_text, temp);
}
void state_set_nmea_boat_depth(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.depth, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dm", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.depth_text, temp);
}
void state_set_nmea_boat_depth_offset(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.depth_offset, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dm", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.depth_offset_text, temp);
}
void state_set_nmea_boat_latitude(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.latitude, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.latitude_text, temp);
}
void state_set_nmea_boat_longitude(int32_t value) {
    lv_subject_set_int(&system_state.nmea.boat.longitude, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d", (int)value);
    lv_subject_set_string(&system_state.nmea.boat.longitude_text, temp);
}
// Environment
void state_set_nmea_environment_temperature_inside(int32_t value) {
    lv_subject_set_int(&system_state.nmea.environment.temperature_inside, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)value);
    lv_subject_set_string(&system_state.nmea.environment.temperature_inside_text, temp);
}
void state_set_nmea_environment_humidity_inside(int32_t value) {
    lv_subject_set_int(&system_state.nmea.environment.humidity_inside, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.nmea.environment.humidity_inside_text, temp);
}
void state_set_nmea_environment_temperature_outside(int32_t value) {
    lv_subject_set_int(&system_state.nmea.environment.temperature_outside, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)value);
    lv_subject_set_string(&system_state.nmea.environment.temperature_outside_text, temp);
}
void state_set_nmea_environment_humidity_outside(int32_t value) {
    lv_subject_set_int(&system_state.nmea.environment.humidity_outside, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.nmea.environment.humidity_outside_text, temp);
}
// -----Victron-----
// House Battery
void state_set_victron_house_battery_voltage(float value){
    lv_subject_set_int(&system_state.victron.house_battery.voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.house_battery.voltage_text, temp);
}
void state_set_victron_house_battery_current(float value){
    lv_subject_set_int(&system_state.victron.house_battery.current, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dA", (int)value);
    lv_subject_set_string(&system_state.victron.house_battery.current_text, temp);
}
void state_set_victron_house_battery_power(float value){
    lv_subject_set_int(&system_state.victron.house_battery.power, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dW", (int)value);
    lv_subject_set_string(&system_state.victron.house_battery.power_text, temp);
}
void state_set_victron_house_battery_soc(float value){
    lv_subject_set_int(&system_state.victron.house_battery.soc, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)value);
    lv_subject_set_string(&system_state.victron.house_battery.soc_text, temp);
}
// Start Battery
void state_set_victron_start_battery_voltage(float value){
    lv_subject_set_int(&system_state.victron.start_battery.voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.start_battery.voltage_text, temp);
}
// IP43 Charger
void state_set_victron_charger_voltage(float value){
    lv_subject_set_int(&system_state.victron.charger.voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.charger.voltage_text, temp);
}
void state_set_victron_charger_current(float value){
    lv_subject_set_int(&system_state.victron.charger.current, value);
    int power = lv_subject_get_int(&system_state.victron.charger.voltage) * value;
    lv_subject_set_int(&system_state.victron.charger.power, power);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dA", (int)value);
    lv_subject_set_string(&system_state.victron.charger.current_text, temp);
    snprintf(temp, sizeof(temp), "%dW", (int)power);
    lv_subject_set_string(&system_state.victron.charger.power_text, temp);
}
// MPPT 1
void state_set_victron_mppt1_dc_voltage(float value){
    lv_subject_set_int(&system_state.victron.mppt1.dc_voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.mppt1.dc_voltage_text, temp);
}
void state_set_victron_mppt1_dc_current(float value){
    lv_subject_set_int(&system_state.victron.mppt1.dc_current, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dA", (int)value);
    lv_subject_set_string(&system_state.victron.mppt1.dc_current_text, temp);
}
void state_set_victron_mppt1_pv_voltage(float value){
    lv_subject_set_int(&system_state.victron.mppt1.pv_voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.mppt1.pv_voltage_text, temp);
}
void state_set_victron_mppt1_yield_today(float value){
    lv_subject_set_int(&system_state.victron.mppt1.yield_today, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkWh", (int)value);
    lv_subject_set_string(&system_state.victron.mppt1.yield_today_text, temp);
}
void state_set_victron_mppt1_power(float value){
    lv_subject_set_int(&system_state.victron.mppt1.power, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dW", (int)value);
    lv_subject_set_string(&system_state.victron.mppt1.power_text, temp);
}
// MPPT 2
void state_set_victron_mppt2_dc_voltage(float value){
    lv_subject_set_int(&system_state.victron.mppt2.dc_voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.mppt2.dc_voltage_text, temp);
}
void state_set_victron_mppt2_dc_current(float value){
    lv_subject_set_int(&system_state.victron.mppt2.dc_current, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dA", (int)value);
    lv_subject_set_string(&system_state.victron.mppt2.dc_current_text, temp);
}
void state_set_victron_mppt2_pv_voltage(float value){
    lv_subject_set_int(&system_state.victron.mppt2.pv_voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.mppt2.pv_voltage_text, temp);
}
void state_set_victron_mppt2_yield_today(float value){
    lv_subject_set_int(&system_state.victron.mppt2.yield_today, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dkWh", (int)value);
    lv_subject_set_string(&system_state.victron.mppt2.yield_today_text, temp);
}
void state_set_victron_mppt2_power(float value){
    lv_subject_set_int(&system_state.victron.mppt2.power, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dW", (int)value);
    lv_subject_set_string(&system_state.victron.mppt2.power_text, temp);
}
// DC2DC
void state_set_victron_dc2dc_voltage(float value){
    lv_subject_set_int(&system_state.victron.dc2dc.voltage, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dV", (int)value);
    lv_subject_set_string(&system_state.victron.dc2dc.voltage_text, temp);
}
void state_set_victron_dc2dc_current(float value){
    lv_subject_set_int(&system_state.victron.dc2dc.current, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dA", (int)value);
    lv_subject_set_string(&system_state.victron.dc2dc.current_text, temp);
}
void state_set_victron_dc2dc_power(float value){
    lv_subject_set_int(&system_state.victron.dc2dc.power, value);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dW", (int)value);
    lv_subject_set_string(&system_state.victron.dc2dc.power_text, temp);
}
// -----System-----
void state_set_system_wifi_status_text(const char * value){
    lv_subject_set_string(&system_state.system.wifi_status_text, value);
}

//keeping this as it's useful code

//void state_set_victron_start_battery_voltage(int value) 
//{
    // Safety check: Filter out corrupted or impossible data before updating LVGL
    //if (millivolts < 0 || millivolts > 60000) {
    //    log_system_error("Voltage out of bounds!");
    //    return; 
    //  }

    // Business Logic: If voltage drops too low, update a separate warning subject
    //if (millivolts < 11500) {
    //    lv_subject_set_int(&sys_state.low_battery_alert_subject, 1); // True
    //} else {
    //    lv_subject_set_int(&sys_state.low_battery_alert_subject, 0); // False
    //}

    // Finally, update the UI value
    //lv_subject_set_int(&system_state.victron.start_battery.voltage, value);
//}
