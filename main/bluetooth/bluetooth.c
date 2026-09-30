#include "bluetooth.h"

//logging
static const char *TAG = "RUUVI_SCANNER";

// Parses Raw Data Format 5 (RAWv2)
void parse_ruuvi_df5(const uint8_t* payload, ruuvi_df5_t* data) {
    int16_t temp_raw = (payload[1] << 8) | payload[2];
    data->temperature = temp_raw * 0.005f;

    uint16_t hum_raw = (payload[3] << 8) | payload[4];
    data->humidity = hum_raw * 0.0025f;

    uint16_t press_raw = (payload[5] << 8) | payload[6];
    data->pressure = (press_raw + 50000) / 100.0f;

    uint16_t power_info = (payload[13] << 8) | payload[14]; // Fixed bounds to bytes 13 & 14
    uint16_t vbat_raw = power_info >> 5;
    data->battery_v = (vbat_raw / 1000.0f) + 1.6f;
}

// Parses Advanced Ruuvi Air (Data Format E1)
void parse_ruuvi_e1(const uint8_t* payload, ruuvi_e1_t* data) {
    // 1. Basic Weather Core
    int16_t temp_raw = (payload[1] << 8) | payload[2];
    data->temperature = (temp_raw == 0x8000) ? 0.0f : temp_raw * 0.005f;

    uint16_t hum_raw = (payload[3] << 8) | payload[4];
    data->humidity = (hum_raw == 0xFFFF) ? 0.0f : hum_raw * 0.0025f;

    uint16_t press_raw = (payload[5] << 8) | payload[6];
    data->pressure = (press_raw == 0xFFFF) ? 0.0f : (press_raw + 50000) / 100.0f;

    // 2. Particulate Matter (PM channels resolution 0.1 per bit)
    data->pm1_0  = ((payload[7] << 8) | payload[8]) * 0.1f;
    data->pm2_5  = ((payload[9] << 8) | payload[10]) * 0.1f;
    data->pm4_0  = ((payload[11] << 8) | payload[12]) * 0.1f;
    data->pm10_0 = ((payload[13] << 8) | payload[14]) * 0.1f;

    // 3. Carbon Dioxide (CO2 ppm)
    data->co2 = (payload[15] << 8) | payload[16];

    // 4. Air Index Factors (VOC and NOx have 9-bit resolution maps)
    uint8_t flags = payload[19]; 
    data->voc = (payload[17] << 1) | ((flags >> 6) & 0x01);
    data->nox = (payload[18] << 1) | ((flags >> 7) & 0x01);

    // 5. Environmental Light Levels (24-bit fixed point)
    uint32_t lux_raw = (payload[20] << 16) | (payload[21] << 8) | payload[22];
    data->luminosity = lux_raw * 0.01f;

    // 6. Packet Sequence Trackers
    data->seq_num = (payload[23] << 16) | (payload[24] << 8) | payload[25];
}

// GAP Event handling for NimBLE Discovery loops
static int ble_gap_event(struct ble_gap_event *event, void *arg) {
    telemetry_packet_t packet; //packet for the queue

    if (event->type == BLE_GAP_EVENT_DISC) {
        struct ble_gap_disc_desc desc = event->disc;
        
        // Use ble_hs_adv_parse to safely walk the AD payload structures
        struct ble_hs_adv_fields fields;
        int rc = ble_hs_adv_parse_fields(&fields, desc.data, desc.length_data);
        if (rc != 0 || fields.mfg_data_len < 2) {
            return 0; // Skip invalid structures
        }

        // Validate Manufacturer Specific Field parameters
        if (fields.mfg_data_len >= 5) {
            // Little-Endian Company ID check
            uint16_t company_id = (fields.mfg_data[1] << 8) | fields.mfg_data[0];
            
            if (company_id == RUUVI_COMPANY_ID) {
                // The Ruuvi payload starts immediately after the 2-byte company ID
                const uint8_t *payload = &fields.mfg_data[2];
                uint8_t format = payload[0];

                if (format == 0x05 && (fields.mfg_data_len - 2) >= 24) {
                    ruuvi_df5_t data;
                    parse_ruuvi_df5(payload, &data);
                    uint64_t tag_id = ((uint64_t)desc.addr.val[5] << 40) |
                        ((uint64_t)desc.addr.val[4] << 32) |
                        ((uint64_t)desc.addr.val[3] << 24) |
                        ((uint64_t)desc.addr.val[2] << 16) |
                        ((uint64_t)desc.addr.val[1] << 8)  |
                        ((uint64_t)desc.addr.val[0]);
                    // Reformat address array into readable hex strings
                    //ESP_LOGI(TAG, "TagID [%02x%02x%02x%02x%02x%02x] RSSI: %d dBm | T: %.2f°C | H: %.2f %% | P: %.2f hPa",
                    //         desc.addr.val[5], desc.addr.val[4], desc.addr.val[3],
                    //         desc.addr.val[2], desc.addr.val[1], desc.addr.val[0],
                    //         desc.rssi, data.temperature, data.humidity, data.pressure, data.battery_v);
                    
                    switch (tag_id) {
                        case 0xe219e9062e2bULL: //AnamCara Cockpit
                            //ESP_LOGI(TAG, "TagID: [0x%012llx] AnamCara Cockpit", tag_id);
                            packet.id = TOPIC_RUUVI_TAG_1_TEMPERATURE;
                            packet.value = (int32_t)data.temperature;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_TAG_1_HUMIDITY;
                            packet.value = (int32_t)data.humidity;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_TAG_1_PRESSURE;
                            packet.value = (int32_t)data.pressure;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                            
                        case 0xf25010624882ULL: //AnamCara Indoors
                            //ESP_LOGI(TAG, "TagID: [0x%012llx] AnamCara Indoors", tag_id);
                            packet.id = TOPIC_RUUVI_TAG_2_TEMPERATURE;
                            packet.value = (int32_t)data.temperature;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_TAG_2_HUMIDITY;
                            packet.value = (int32_t)data.humidity;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_TAG_2_PRESSURE;
                            packet.value = (int32_t)data.pressure;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                            
                        default:
                            ESP_LOGW(TAG, "Unknown Tag: 0x%012llx Unknown", tag_id);
                            break;
                    }

                }
                // Air Quality Sensor (Accepts BOTH Bluetooth 4 Format 0x06 AND Bluetooth 5 Format 0xE1)
                else if ((format == 0x06 || format == 0xE1) && (fields.mfg_data_len - 2) >= 20) {
                    ruuvi_e1_t air;
                    parse_ruuvi_e1(payload, &air);
                    uint64_t tag_id = ((uint64_t)desc.addr.val[5] << 40) |
                        ((uint64_t)desc.addr.val[4] << 32) |
                        ((uint64_t)desc.addr.val[3] << 24) |
                        ((uint64_t)desc.addr.val[2] << 16) |
                        ((uint64_t)desc.addr.val[1] << 8)  |
                        ((uint64_t)desc.addr.val[0]);
                    //ESP_LOGW(TAG, "AirID [%02x%02x%02x%02x%02x%02x] RSSI: %d dBm | T: %.2f°C | H: %.2f %% | P: %.2f hPa | CO2: %d ppm | PM2.5: %.1f ug/m³ | VOC Index: %d",
                    // desc.addr.val[5], desc.addr.val[4], desc.addr.val[3],
                    // desc.addr.val[2], desc.addr.val[1], desc.addr.val[0],
                    // desc.rssi, air.temperature, air.humidity, air.pressure, air.co2, air.pm2_5, air.voc);
                    switch (tag_id) {
                        case 0xeb807f705dc3ULL: //AnamCara Air
                            //ESP_LOGI(TAG, "TagID: [0x%012llx] AnamCara Air", tag_id);
                            packet.id = TOPIC_RUUVI_AIR_1_TEMPERATURE;
                            packet.value = (int32_t)air.temperature;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_HUMIDITY;
                            packet.value = (int32_t)air.humidity;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_PRESSURE;
                            packet.value = (int32_t)air.pressure;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_CO2;
                            packet.value = (int32_t)air.co2;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_NOX;
                            packet.value = (int32_t)air.nox;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_PM2_5;
                            packet.value = (int32_t)air.pm2_5;
                            xQueueSend(msg_queue, &packet, 0);
                            packet.id = TOPIC_RUUVI_AIR_1_VOC;
                            packet.value = (int32_t)air.voc;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        default:
                            ESP_LOGW(TAG, "Unknown Air Tag: 0x%012llx Unknown", tag_id);
                            break;
                    }
                }
                
            }
        }
    }
    return 0;
}

static void ble_app_scan(void) {
    struct ble_gap_disc_params disc_params;
    memset(&disc_params, 0, sizeof(disc_params));
    
    disc_params.filter_duplicates = 0; // Disable to continuously monitor metrics
    disc_params.passive = 1;           // Passive scan mode (no scan request queries)
    disc_params.itvl = 0x50;           // Interval 50ms
    disc_params.window = 0x30;         // Window 30ms

    // Start discovery scan cycle loop
    int rc = ble_gap_disc(BLE_OWN_ADDR_PUBLIC, BLE_HS_FOREVER, &disc_params, ble_gap_event, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to initiate discovery; error code = %d", rc);
    }
}

void ble_host_task(void *param) {
    ESP_LOGI(TAG, "NimBLE Host Task Thread Started.");
    nimble_port_run(); // Blocks execution here until explicitly shutdown
    nimble_port_freertos_deinit();
}

void bluetooth_init() {
    // Initialize the underlying controller hardware layer
    ESP_ERROR_CHECK(nimble_port_init());
    
    // Set host synchronization defaults 
    ble_hs_cfg.sync_cb = ble_app_scan;
}