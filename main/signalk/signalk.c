#include "esp_websocket_client.h"
#include "signalk.h"
#include "cJSON.h"
#include "esp_log.h"

static const char *TAG = "Boat Assistant: SignalK Victron/NMEA2K";
static esp_websocket_client_handle_t client;

// Keep a persistent pointer to build fragmented payloads
static char *rx_buffer = NULL;
static int rx_buffer_len = 0;

// WEBSOCKET EVENT HANDLER PATTERN
static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    telemetry_packet_t packet; //packet for the queue

    switch (event_id) {
        case WEBSOCKET_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to Cerbo SignalK");
            // Send the JSON message over the open WebSocket frame
            int bytes_sent = esp_websocket_client_send_text(
                data->client, 
                SIGNALK_SUBSCRIPTION_PLAN, 
                strlen(SIGNALK_SUBSCRIPTION_PLAN), 
                pdMS_TO_TICKS(1000) // Timeout after 1 second if the buffer is blocked
            );
            // Connection established! Ready to receive Signal K stream.
            break;
        case WEBSOCKET_EVENT_DATA:
            // Process text frames (op_code 0x01)
            if (data->op_code == 0x01 && data->data_len > 0) {
                // Handle the start of a new message or packet segment
                if (data->payload_offset == 0) {
                    // Prepare a clean buffer for the full expected payload size
                    // Allocating data->payload_len + 1 to leave room for a null terminator
                    rx_buffer = malloc(data->payload_len + 1);
                    if (rx_buffer == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for WebSocket payload");
                        break;
                    }
                    rx_buffer_len = 0;
                }
                // Append the current chunk into our accumulator buffer
                if (rx_buffer != NULL) {
                    memcpy(rx_buffer + data->payload_offset, data->data_ptr, data->data_len);
                    rx_buffer_len += data->data_len;
                    // Check if we have received the final chunk of this message
                    if (rx_buffer_len >= data->payload_len) {
                        rx_buffer[data->payload_len] = '\0'; // Inject safety null-terminator
            // Ensure this is a completed text frame (following the reassembly logic from the previous step)
                        // For this snippet, 'rx_buffer' contains the full, null-terminated JSON string.

                        cJSON *root = cJSON_Parse(rx_buffer);
                        if (root == NULL) {
                            ESP_LOGE(TAG, "Failed to parse incoming Signal K JSON");
                            break;
                        }

                        // 1. Dig into the Signal K array structure: updates -> values
                        cJSON *updates = cJSON_GetObjectItemCaseSensitive(root, "updates");
                        if (!cJSON_IsArray(updates)) {
                            cJSON_Delete(root);
                            break;
                        }

                        cJSON *update_item = NULL;
                        cJSON_ArrayForEach(update_item, updates) {
                            cJSON *values = cJSON_GetObjectItemCaseSensitive(update_item, "values");
                            if (!cJSON_IsArray(values)) {
                                continue; 
                            }

                            cJSON *value_item = NULL;
                            cJSON_ArrayForEach(value_item, values) {
                                cJSON *path_node = cJSON_GetObjectItemCaseSensitive(value_item, "path");
                                cJSON *value_node = cJSON_GetObjectItemCaseSensitive(value_item, "value");

                                if (cJSON_IsString(path_node) && (value_node != NULL)) {
                                    const char *current_path = path_node->valuestring;
                                    size_t current_path_len = strlen(current_path);

                                    // 2. Identify which Signal K Path this is (Your Loop Registry Pattern)
                                    signalk_topic_id_t matched_id = SIGNALK_TOPIC_COUNT;

                                    for (int i = 0; i < SIGNALK_TOPIC_COUNT; i++) {
                                        // Compare lengths first, then string contents for safety against your new registry
                                        size_t registry_string_len = strlen(signalk_topic_registry[i]);
                                        if (current_path_len == registry_string_len) {
                                            if (strncmp(current_path, signalk_topic_registry[i], current_path_len) == 0) {
                                                matched_id = (signalk_topic_id_t)i;
                                                break;
                                            }
                                        }
                                    }

                                    // If it's a path we don't care about, skip to next value item
                                    if (matched_id == SIGNALK_TOPIC_COUNT) {
                                        continue;
                                    }

                                    // 3. Extracted numeric processing inside your clean switch block
                                    if (cJSON_IsNumber(value_node)) {
                                        switch (matched_id) {
                                            case SIGNALK_TOPIC_NAVIGATION_SOG: {
                                                packet.id = TOPIC_NMEA_BOAT_SPEED_OVER_GROUND;
                                                packet.value.value_float = (float)value_node->valuedouble;
                                                xQueueSend(msg_queue, &packet, 0);
                                                break;
                                            }
                                            case SIGNALK_TOPIC_ENVIRONMENT_DEPTH: {
                                                ESP_LOGI(TAG, "Depth Signal K (%.1fm):\n", (float)value_node->valuedouble);
                                                packet.id = TOPIC_NMEA_BOAT_DEPTH;
                                                packet.value.value_float = (float)value_node->valuedouble;
                                                xQueueSend(msg_queue, &packet, 0);
                                                break;
                                            }
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                    

                        // --- THE DATA IS READY FOR USE HERE ---
                        //ESP_LOGI(TAG, "Complete Message Received (%d bytes)", data->payload_len);
                        

                        cJSON_Delete(root);
                        // Clean up memory after processing the packet
                        free(rx_buffer);
                        rx_buffer = NULL;
                    }
                }
            }
            // Handle incoming data payload: data->data_ptr

            //debugging, show all packets in log
            // Check if it's a text frame (0x01) and contains actual data
            //if (data->op_code == 0x01 && data->data_len > 0) {
            //    // Use precision format '%.*s' to safely print non-null-terminated buffers
            //    ESP_LOGI(TAG, "Raw Signal K Packet (%d bytes):\n%.*s", 
            //            data->data_len, 
            //            data->data_len, 
            //            (char *)data->data_ptr);
            //}
            break;
        case WEBSOCKET_EVENT_DISCONNECTED:
            // Prevent memory leaks if the connection drops mid-stream
            if (rx_buffer != NULL) {
                free(rx_buffer);
                rx_buffer = NULL;
            }
            ESP_LOGW(TAG, "Disconnected from Signal K Server.");
            break;
    }
}

void signalk_app_start(void) {
    esp_websocket_client_config_t websocket_cfg = {
        .uri = CERBO_WS_URI, // Cerbo GX IP
    };
    client = esp_websocket_client_init(&websocket_cfg);
    // Register the event handler
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    esp_websocket_client_start(client);
}