
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
                state_set_ruuvi_tag_1_temperature(packet.value);
                break;
            case TOPIC_RUUVI_TAG_1_HUMIDITY:
                state_set_ruuvi_tag_1_humidity(packet.value);
                break;
            case TOPIC_RUUVI_TAG_1_PRESSURE:
                state_set_ruuvi_tag_1_pressure(packet.value);
                break;
            // Ruuvi Tag 2
            case TOPIC_RUUVI_TAG_2_TEMPERATURE:
                state_set_ruuvi_tag_2_temperature(packet.value);
                break;
            case TOPIC_RUUVI_TAG_2_HUMIDITY:
                state_set_ruuvi_tag_2_humidity(packet.value);
                break;
            case TOPIC_RUUVI_TAG_2_PRESSURE:
                state_set_ruuvi_tag_2_pressure(packet.value);
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
// -----Victron-----
            // House Battery
            case TOPIC_HOUSE_BATTERY_VOLTAGE:
                break;
            case TOPIC_HOUSE_BATTERY_CURRENT:
                break;
            case TOPIC_HOUSE_BATTERY_POWER:
                break;
            case TOPIC_HOUSE_BATTERY_SOC:
                break;
            // Start Battery
            case TOPIC_START_BATTERY_VOLTAGE:
                break;
// -----System-----
            case TOPIC_SYSTEM_WIFI_CONNECTED:
                break;
            case TOPIC_SYSTEM_WIFI_IP_ADDRESS:
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