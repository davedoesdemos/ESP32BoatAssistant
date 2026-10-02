#include "queue_reader.h"

QueueHandle_t msg_queue = NULL;

static void queue_reader_cb(lv_timer_t * timer) 
{
    QueueHandle_t data_queue = (QueueHandle_t)lv_timer_get_user_data(timer);
    telemetry_packet_t packet;

    // Pull every single pending message off the queue
    while (xQueueReceive(data_queue, &packet, 0) == pdTRUE) {
        // Route data safely using the telemetry ID
        switch (packet.id) {
// -----Ruuvi-----
            // Ruuvi Tag 1
            case TOPIC_RUUVI_TAG_1_TEMPERATURE:
                state_set_ruuvi_tag_1_temperature(packet.value.value_int);
                break;
            case TOPIC_RUUVI_TAG_1_HUMIDITY:
                state_set_ruuvi_tag_1_humidity(packet.value.value_int);
                break;
            case TOPIC_RUUVI_TAG_1_PRESSURE:
                state_set_ruuvi_tag_1_pressure(packet.value.value_int);
                break;
            // Ruuvi Tag 2
            case TOPIC_RUUVI_TAG_2_TEMPERATURE:
                state_set_ruuvi_tag_2_temperature(packet.value.value_int);
                break;
            case TOPIC_RUUVI_TAG_2_HUMIDITY:
                state_set_ruuvi_tag_2_humidity(packet.value.value_int);
                break;
            case TOPIC_RUUVI_TAG_2_PRESSURE:
                state_set_ruuvi_tag_2_pressure(packet.value.value_int);
                break;
            // Ruuvi Air 1
            case TOPIC_RUUVI_AIR_1_TEMPERATURE:
                state_set_ruuvi_air_1_temperature(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_HUMIDITY:
                state_set_ruuvi_air_1_humidity(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_PRESSURE:
                state_set_ruuvi_air_1_pressure(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_PM2_5:
                state_set_ruuvi_air_1_pm2_5(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_CO2:
                state_set_ruuvi_air_1_co2(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_VOC:
                state_set_ruuvi_air_1_voc(packet.value.value_int);
                break;
            case TOPIC_RUUVI_AIR_1_NOX:
                state_set_ruuvi_air_1_nox(packet.value.value_int);
                break;
// -----NMEA2k-----
            // Wind
            case TOPIC_NMEA_WIND_TRUE_SPEED:
                state_set_nmea_wind_true_speed(packet.value.value_int);
                break;
            case TOPIC_NMEA_WIND_TRUE_DIRECTION:
                state_set_nmea_wind_true_direction(packet.value.value_int);
                break;
            case TOPIC_NMEA_WIND_APPARENT_SPEED:
                state_set_nmea_wind_apparent_speed(packet.value.value_int);
                break;
            case TOPIC_NMEA_WIND_APPARENT_DIRECTION:
                state_set_nmea_wind_apparent_direction(packet.value.value_int);
                break;
            // Diesel Tank
            case TOPIC_NMEA_DIESELTANK_LEVEL_PERCENT:
                state_set_nmea_dieseltank_level_percent(packet.value.value_int);
                break;
            case TOPIC_NMEA_DIESELTANK_LEVEL_LITRES:
                state_set_nmea_dieseltank_level_litres(packet.value.value_int);
                break;
            case TOPIC_NMEA_DIESELTANK_CAPACITY:
                state_set_nmea_dieseltank_capacity(packet.value.value_int);
            // Boat
            case TOPIC_NMEA_BOAT_HEADING:
                state_set_nmea_boat_heading(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_COURSE_OVER_GROUND:
                state_set_nmea_boat_course_over_ground(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_SPEED_OVER_GROUND:
                state_set_nmea_boat_speed_over_ground(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_SPEED_THROUGH_WATER:
                state_set_nmea_boat_speed_through_water(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_DEPTH:
                state_set_nmea_boat_depth(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_DEPTH_OFFSET:
                state_set_nmea_boat_depth_offset(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_LATITUDE:
                state_set_nmea_boat_latitude(packet.value.value_int);
                break;
            case TOPIC_NMEA_BOAT_LONGITUDE:
                state_set_nmea_boat_longitude(packet.value.value_int);
                break;
            // Environment
            case TOPIC_NMEA_ENVIRONMENT_TEMPERATURE_INSIDE:
                state_set_nmea_environment_temperature_inside(packet.value.value_int);
                break;
            case TOPIC_NMEA_ENVIRONMENT_HUMIDITY_INSIDE:
                state_set_nmea_environment_humidity_inside(packet.value.value_int);
                break;
            case TOPIC_NMEA_ENVIRONMENT_TEMPERATURE_OUTSIDE:
                state_set_nmea_environment_temperature_outside(packet.value.value_int);
                break;
            case TOPIC_NMEA_ENVIRONMENT_HUMIDITY_OUTSIDE:
                state_set_nmea_environment_humidity_outside(packet.value.value_int);
                break;
// -----Victron-----
            // House Battery
            case TOPIC_VICTRON_HOUSE_BATTERY_VOLTAGE:
                state_set_victron_house_battery_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_HOUSE_BATTERY_CURRENT:
                state_set_victron_house_battery_current(packet.value.value_float);
                break;
            case TOPIC_VICTRON_HOUSE_BATTERY_POWER:
                state_set_victron_house_battery_power(packet.value.value_float);
                break;
            case TOPIC_VICTRON_HOUSE_BATTERY_SOC:
                state_set_victron_house_battery_soc(packet.value.value_float);
                break;
            // Start Battery
            case TOPIC_VICTRON_START_BATTERY_VOLTAGE:
                state_set_victron_start_battery_voltage(packet.value.value_float);
                break;
            // IP43 charger
            case TOPIC_VICTRON_CHARGER_VOLTAGE:
                state_set_victron_charger_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_CHARGER_CURRENT:
                state_set_victron_charger_current(packet.value.value_float);
                break;
            // MPPT1
            case TOPIC_VICTRON_MPPT1_DC_VOLTAGE:
                state_set_victron_mppt1_dc_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT1_DC_CURRENT:
                state_set_victron_mppt1_dc_current(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT1_PV_VOLTAGE:
                state_set_victron_mppt1_pv_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT1_YIELD_TODAY:
                state_set_victron_mppt1_yield_today(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT1_POWER:
                state_set_victron_mppt1_power(packet.value.value_float);
                break;
            // MPPT2
            case TOPIC_VICTRON_MPPT2_DC_VOLTAGE:
                state_set_victron_mppt2_dc_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT2_DC_CURRENT:
                state_set_victron_mppt2_dc_current(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT2_PV_VOLTAGE:
                state_set_victron_mppt2_pv_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT2_YIELD_TODAY:
                state_set_victron_mppt2_yield_today(packet.value.value_float);
                break;
            case TOPIC_VICTRON_MPPT2_POWER:
                state_set_victron_mppt2_power(packet.value.value_float);
                break;
            // DC2DC
            case TOPIC_VICTRON_DC2DC_VOLTAGE:
                state_set_victron_dc2dc_voltage(packet.value.value_float);
                break;
            case TOPIC_VICTRON_DC2DC_CURRENT:
                state_set_victron_dc2dc_current(packet.value.value_float);
                break;
            case TOPIC_VICTRON_DC2DC_POWER:
                state_set_victron_dc2dc_power(packet.value.value_float);
                break;
// -----System-----
            case TOPIC_SYSTEM_WIFI_CONNECTED:
                break;
            case TOPIC_SYSTEM_WIFI_STATUS:
                break;
// -----Default-----
            default:
                // Unknown packet ID safety catch
                break;
        }
    }
}

void queue_reader_init(QueueHandle_t hardware_queue) 
{
    // Check for new messages every 30ms
    lv_timer_create(queue_reader_cb, 30, (void*)hardware_queue);
}