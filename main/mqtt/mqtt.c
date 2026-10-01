#include "mqtt_client.h"
#include "esp_log.h"
#include "cJSON.h" // Required to parse Victron's payload wrapper
#include "mqtt.h"

static const char *TAG = "VICTRON_MQTT";
static esp_mqtt_client_handle_t client;

// Define our target topics
#define KEEPALIVE_TOPIC "R/" VRM_ID "/keepalive"


void victron_keepalive_task(void *pvParameters) {
    while (1) {
        // Send an empty string or empty payload to reset the Cerbo's timeout
        int msg_id = esp_mqtt_client_publish(client, KEEPALIVE_TOPIC, "", 0, 1, 0);
        if (msg_id != -1) {
            ESP_LOGD(TAG, "Sent keepalive packet to Cerbo GX");
        } else {
            ESP_LOGE(TAG, "Failed to send keepalive");
        }
        // Delay for 30 seconds (Victron times out at 60 seconds)
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    telemetry_packet_t packet; //packet for the queue
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to Cerbo GX Broker!");
            
            // 1. Loop through your registry and subscribe to every topic automatically
            for (int i = 0; i < TOPIC_COUNT; i++) {
                int msg_id = esp_mqtt_client_subscribe(client, topic_registry[i], 0);
                if (msg_id != -1) {
                    ESP_LOGI(TAG, "Successfully subscribed to: %s", topic_registry[i]);
                } else {
                    ESP_LOGE(TAG, "Failed to subscribe to: %s", topic_registry[i]);
                }
            }
            
            // 2. Start the keepalive task immediately upon connection
            xTaskCreate(victron_keepalive_task, "victron_ka", 2048, NULL, 5, NULL);
            break;

        case MQTT_EVENT_DATA:
            // 1. Identify which topic this is
            victron_topic_id_t matched_id = TOPIC_COUNT;

            for (int i = 0; i < TOPIC_COUNT; i++) {
                // Compare lengths first, then string contents for safety
                size_t registry_string_len = strlen(topic_registry[i]);
                if (event->topic_len ==registry_string_len) {
                    if (strncmp(event->topic, topic_registry[i], event->topic_len) == 0) {
                        matched_id = (victron_topic_id_t)i; // Loop counter becomes the Enum ID
                        break;
                    }
                }
            }
            // If it's a topic we don't care about, exit early
            if (matched_id == TOPIC_COUNT) {
                break;
            }
            // 2. Safely extract and parse the JSON string wrapper
            char *json_string = malloc(event->data_len + 1);
            snprintf(json_string, event->data_len + 1, "%s", event->data);
            cJSON *root = cJSON_Parse(json_string);
            if (root != NULL) {
                cJSON *value_node = cJSON_GetObjectItem(root, "value");
                if (cJSON_IsNumber(value_node)) {
                    // 3. Clean, readable switch statement!
                    switch (matched_id) {
                        // House Battery
                        case TOPIC_HOUSE_BATTERY_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_HOUSE_BATTERY_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            //ESP_LOGI(TAG, "House Battery Voltage: %.2f V", house_battery_voltage);
                            break;
                        }
                        case TOPIC_HOUSE_BATTERY_CURRENT: {
                            packet.id = TOPIC_VICTRON_HOUSE_BATTERY_CURRENT;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_HOUSE_BATTERY_POWER: {
                            packet.id = TOPIC_VICTRON_HOUSE_BATTERY_POWER;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_HOUSE_BATTERY_SOC: {
                            packet.id = TOPIC_VICTRON_HOUSE_BATTERY_SOC;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        // Start Battery
                        case TOPIC_START_BATTERY_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_START_BATTERY_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        // IP43 charger
                        case TOPIC_CHARGER_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_CHARGER_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_CHARGER_CURRENT: {
                            packet.id = TOPIC_VICTRON_CHARGER_CURRENT;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        // MPPT1
                        case TOPIC_MPPT1_DC_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_MPPT1_DC_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT1_DC_CURRENT: {
                            packet.id = TOPIC_VICTRON_MPPT1_DC_CURRENT;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT1_PV_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_MPPT1_PV_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT1_YIELD_TODAY: {
                            packet.id = TOPIC_VICTRON_MPPT1_YIELD_TODAY;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT1_POWER: {
                            packet.id = TOPIC_VICTRON_MPPT1_POWER;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        // MPPT2
                        case TOPIC_MPPT2_DC_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_MPPT2_DC_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT2_DC_CURRENT: {
                            packet.id = TOPIC_VICTRON_MPPT2_DC_CURRENT;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT2_PV_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_MPPT2_PV_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT2_YIELD_TODAY: {
                            packet.id = TOPIC_VICTRON_MPPT2_YIELD_TODAY;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_MPPT2_POWER: {
                            packet.id = TOPIC_VICTRON_MPPT2_POWER;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        // DC2DC
                        case TOPIC_DC2DC_VOLTAGE: {
                            packet.id = TOPIC_VICTRON_DC2DC_VOLTAGE;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_DC2DC_CURRENT: {
                            packet.id = TOPIC_VICTRON_DC2DC_CURRENT;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        case TOPIC_DC2DC_POWER: {
                            packet.id = TOPIC_VICTRON_DC2DC_POWER;
                            packet.value.value_float = value_node->valuedouble;
                            xQueueSend(msg_queue, &packet, 0);
                            break;
                        }
                        default:
                            break;
                    }
                }
                cJSON_Delete(root);
            }
            free(json_string);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Disconnected from Cerbo GX broker");
            break;
        default:
            break;
    }
}
void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://192.168.1.2:1883", // Cerbo GX IP
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}