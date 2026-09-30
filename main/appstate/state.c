#include "state.h"

// Inside state.c (Hidden from the rest of the application)
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
            lv_subject_t humidity;
            lv_subject_t pressure;
            lv_subject_t pm2_5; //Particulate Matter (PM) 2.5µm
            lv_subject_t co2; //Carbon Dioxide (CO₂)
            lv_subject_t voc; //VOC Volatile Organic Compounds
            lv_subject_t nox; //NOx Nitrogen Oxides
        } air_1;
    } ruuvi;
    // -----Victron-----
    struct {
        // House Battery
        struct {
            lv_subject_t voltage;
            lv_subject_t current;
            lv_subject_t power;
            lv_subject_t soc;
        } house_battery;
        // Start Battery
        struct {
            lv_subject_t voltage;
        } start_battery;
        // IP43 charger
        struct {
            lv_subject_t voltage;
            lv_subject_t current;
        } charger;
        // MPPT1
        struct {
            lv_subject_t dc_voltage;
            lv_subject_t dc_current;
            lv_subject_t pv_voltage;
            lv_subject_t yield_today;
        } mppt1;
        // MPPT2
        struct {
            lv_subject_t dc_voltage;
            lv_subject_t dc_current;
            lv_subject_t pv_voltage;
            lv_subject_t yield_today;
        } mppt2;
        // DC2DC
        struct {
            lv_subject_t voltage;
            lv_subject_t current;
            lv_subject_t power;
        } dc2dc;
    } victron;
    // -----System-----
    struct {
        lv_subject_t wifi_connected;
        lv_subject_t wifi_ip_address;
    } system;
} system_state_t;

static system_state_t system_state; // The single source of truth

//link the externals to the state struct
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
    lv_subject_t * const state_ruuvi_air_1_humidity = &system_state.ruuvi.air_1.humidity;
    lv_subject_t * const state_ruuvi_air_1__pressure = &system_state.ruuvi.air_1.pressure;
    lv_subject_t * const state_ruuvi_air_1_pm2_5 = &system_state.ruuvi.air_1.pm2_5; //Particulate Matter (PM) 2.5µm
    lv_subject_t * const state_ruuvi_air_1_co2 = &system_state.ruuvi.air_1.co2; //Carbon Dioxide (CO₂)
    lv_subject_t * const state_ruuvi_air_1_voc = &system_state.ruuvi.air_1.voc; //VOC Volatile Organic Compounds
    lv_subject_t * const state_ruuvi_air_1_nox = &system_state.ruuvi.air_1.nox; //NOx Nitrogen Oxides
// -----Victron-----
    // House Battery
    lv_subject_t * const state_house_battery_voltage = &system_state.victron.house_battery.voltage;
    lv_subject_t * const state_house_battery_current = &system_state.victron.house_battery.current;
    lv_subject_t * const state_house_battery_power = &system_state.victron.house_battery.power;
    lv_subject_t * const state_house_battery_soc = &system_state.victron.house_battery.soc;
    // Start Battery
    lv_subject_t * const state_start_battery_voltage = &system_state.victron.start_battery.voltage;
    // IP43 Charger
    lv_subject_t * const state_charger_voltage = &system_state.victron.charger.voltage;
    lv_subject_t * const state_charger_current = &system_state.victron.charger.current;
    // MPPT 1
    lv_subject_t * const state_mppt1_dc_voltage = &system_state.victron.mppt1.dc_voltage;
    lv_subject_t * const state_mppt1_dc_current = &system_state.victron.mppt1.dc_current;
    lv_subject_t * const state_mppt1_pv_voltage = &system_state.victron.mppt1.pv_voltage;
    lv_subject_t * const state_mppt1_yield_today = &system_state.victron.mppt1.yield_today;
    // MPPT 1
    lv_subject_t * const state_mppt2_dc_voltage = &system_state.victron.mppt2.dc_voltage;
    lv_subject_t * const state_mppt2_dc_current = &system_state.victron.mppt2.dc_current;
    lv_subject_t * const state_mppt2_pv_voltage = &system_state.victron.mppt2.pv_voltage;
    lv_subject_t * const state_mppt2_yield_today = &system_state.victron.mppt2.yield_today;
    // DC2DC
    lv_subject_t * const state_dc2dc_voltage = &system_state.victron.dc2dc.voltage;
    lv_subject_t * const state_dc2dc_current = &system_state.victron.dc2dc.current;
    lv_subject_t * const state_dc2dc_power = &system_state.victron.dc2dc.power;
// -----System-----
    lv_subject_t * const state_system_wifi_connected = &system_state.system.wifi_connected;
    lv_subject_t * const state_system_wifi_ip_address = &system_state.system.wifi_ip_address;

//char buffers for text states
static char state_ruuvi_tag_1_temperature_text_buf[16] = "---°C";
static char state_ruuvi_tag_1_temperature_text_prev_buf[16] = "---°C";
static char state_ruuvi_tag_1_humidity_text_buf[16] = "---%";
static char state_ruuvi_tag_1_humidity_text_prev_buf[16] = "---%";
static char state_ruuvi_tag_1_pressure_text_buf[16] = "---hPa";
static char state_ruuvi_tag_1_pressure_text_prev_buf[16] = "---hPa";
static char state_ruuvi_tag_2_temperature_text_buf[16] = "---°C";
static char state_ruuvi_tag_2_temperature_text_prev_buf[16] = "---°C";
static char state_ruuvi_tag_2_humidity_text_buf[16] = "---%";
static char state_ruuvi_tag_2_humidity_text_prev_buf[16] = "---%";
static char state_ruuvi_tag_2_pressure_text_buf[16] = "---hPa";
static char state_ruuvi_tag_2_pressure_text_prev_buf[16] = "---hPa";

void state_init(void) 
{
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
    
    // -----Victron-----
        // House Battery
        lv_subject_init_int(&system_state.victron.house_battery.voltage, 0);
        lv_subject_init_int(&system_state.victron.house_battery.current, 0);
        lv_subject_init_int(&system_state.victron.house_battery.power, 0);
        lv_subject_init_int(&system_state.victron.house_battery.soc,     100); // Start at 100%
        // Start Battery
        lv_subject_init_int(&system_state.victron.start_battery.voltage,       0);
}

void state_set_ruuvi_tag_1_temperature(int32_t temperature){
    lv_subject_set_int(&system_state.ruuvi.tag_1.temperature, temperature);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)temperature);
    lv_subject_set_string(&system_state.ruuvi.tag_1.temperature_text, temp);
}

void state_set_ruuvi_tag_1_humidity(int32_t percentage){
    lv_subject_set_int(&system_state.ruuvi.tag_1.humidity, percentage);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)percentage);
    lv_subject_set_string(&system_state.ruuvi.tag_1.humidity_text, temp);
}

void state_set_ruuvi_tag_1_pressure(int32_t hpa){
    lv_subject_set_int(&system_state.ruuvi.tag_1.pressure, hpa);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dhPa", (int)hpa);
    lv_subject_set_string(&system_state.ruuvi.tag_1.pressure_text, temp);
}

void state_set_ruuvi_tag_2_temperature(int32_t temperature){
    lv_subject_set_int(&system_state.ruuvi.tag_2.temperature, temperature);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d°C", (int)temperature);
    lv_subject_set_string(&system_state.ruuvi.tag_2.temperature_text, temp);
}

void state_set_ruuvi_tag_2_humidity(int32_t percentage){
    lv_subject_set_int(&system_state.ruuvi.tag_2.humidity, percentage);
    char temp[16];
    snprintf(temp, sizeof(temp), "%d%%", (int)percentage);
    lv_subject_set_string(&system_state.ruuvi.tag_2.humidity_text, temp);
}

void state_set_ruuvi_tag_2_pressure(int32_t hpa){
    lv_subject_set_int(&system_state.ruuvi.tag_2.pressure, hpa);
    char temp[16];
    snprintf(temp, sizeof(temp), "%dhPa", (int)hpa);
    lv_subject_set_string(&system_state.ruuvi.tag_2.pressure_text, temp);
}

void state_set_victron_start_battery_voltage(int millivolts) 
{
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
    lv_subject_set_int(&system_state.victron.start_battery.voltage, millivolts);
}