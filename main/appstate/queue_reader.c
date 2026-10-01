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
                break;
            case TOPIC_RUUVI_AIR_1_HUMIDITY:
                break;
            case TOPIC_RUUVI_AIR_1_PRESSURE:
                break;
            case TOPIC_RUUVI_AIR_1_PM2_5: //Particulate Matter (PM) 2.5µm
                break;
            case TOPIC_RUUVI_AIR_1_CO2: //Carbon Dioxide (CO₂)
                break;
            case TOPIC_RUUVI_AIR_1_VOC: //VOC Volatile Organic Compounds
                break;
            case TOPIC_RUUVI_AIR_1_NOX: //NOx Nitrogen Oxides
                break;
// -----NMEA2k-----
            // Wind
            case TOPIC_NMEA_WIND_TRUE_SPEED:
                break;
            case TOPIC_NMEA_WIND_TRUE_DIRECTION:
                break;
            case TOPIC_NMEA_WIND_APPARENT_SPEED:
                break;
            case TOPIC_NMEA_WIND_APPARENT_DIRECTION:
                break;
            // Diesel Tank
            case TOPIC_NMEA_DIESELTANK_LEVEL_PERCENT:
                break;
            case TOPIC_NMEA_DIESELTANK_LEVEL_LITRES:
                break;
            case TOPIC_NMEA_DIESELTANK_CAPACITY:
            // Boat
            case TOPIC_NMEA_BOAT_HEADING:
                break;
            case TOPIC_NMEA_BOAT_COURSE_OVER_GROUND:
                break;
            case TOPIC_NMEA_BOAT_SPEED_OVER_GROUND:
                break;
            case TOPIC_NMEA_BOAT_SPEED_THROUGH_WATER:
                break;
            case TOPIC_NMEA_BOAT_DEPTH:
                break;
            case TOPIC_NMEA_BOAT_DEPTH_OFFSET:
                break;
            case TOPIC_NMEA_BOAT_LATITUDE:
                break;
            case TOPIC_NMEA_BOAT_LONGITUDE:
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