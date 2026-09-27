#include "nmea2k.h"

//logging
static const char *TAG = "boat assistant nmea";

// Define a structural format for the PGN directory
typedef struct {
    uint32_t pgn;
    const char *label;
    bool is_fast_packet; // Flags if the message splits across multiple CAN frames
} n2k_pgn_meta_t;

// Populate the structural database using the standardized CANboat definitions
static const n2k_pgn_meta_t pgn_directory[] = {
    // --- System / Network Management PGNs ---
    { 59392,  "ISO Acknowledgment", false },
    { 59904,  "ISO Request", false },
    { 60160,  "ISO Transport Protocol (Data)", false },
    { 60416,  "ISO Transport Protocol (Connection)", false },
    { 60928,  "ISO Address Claim", false },
    { 65240,  "ISO Commanded Address", true },
    { 126208, "NMEA Group Function", true },
    { 126464, "PGN List (Tx/Rx)", true },
    { 126992, "System Time", false },
    { 126993, "Heartbeat", false },
    { 126996, "Product Information", true },
    { 126998, "Configuration Information", true },

    // --- Navigation / Attitude PGNs ---
    { 127237, "Heading/Track Control", false },
    { 127245, "Rudder", false },
    { 127250, "Vessel Heading", false },
    { 127251, "Rate of Turn", false },
    { 127257, "Attitude", false },
    { 127258, "Magnetic Variation", false },

    // --- Engine / Propulsion PGNs ---
    { 127488, "Engine Parameters, Rapid Update", false },
    { 127489, "Engine Parameters, Dynamic", true },
    { 127493, "Transmission Parameters, Dynamic", false },
    { 127498, "Engine Parameters, Static", true },

    // --- Fluid / Power / DC PGNs ---
    { 127501, "Binary Status Report", true },
    { 127502, "Switch Bank Control", true },
    { 127505, "Fluid Level", false },
    { 127506, "DC Detailed Status", true },
    { 127508, "Battery Status", false },

    // --- GPS / Navigation Data PGNs ---
    { 128259, "Speed, Water Referenced", false },
    { 128267, "Water Depth", false },
    { 128275, "Distance Log", false },
    { 129025, "Position, Rapid Update", false },
    { 129026, "COG & SOG, Rapid Update", false },
    { 129029, "GNSS Position Data", true },
    { 129283, "Cross Track Error", false },
    { 129284, "Navigation Data", true },
    { 129285, "Navigation - Route/WP Information", true },

    // --- AIS PGNs (Almost strictly Fast-Packet) ---
    { 129038, "AIS Class A Position Report", true },
    { 129039, "AIS Class B Position Report", true },
    { 129040, "AIS Class B Extended Position Report", true },
    { 129794, "AIS Class A Static and Voyage Data", true },
    { 129809, "AIS Class B CS Static Data Report, Part A", true },
    { 129810, "AIS Class B CS Static Data Report, Part B", true },

    // --- Environmental PGNs ---
    { 130310, "Environmental Parameters", false },
    { 130311, "Environmental Parameters (Obsolete)", false },
    { 130312, "Temperature", false },
    { 130313, "Humidity", false },
    { 130314, "Actual Pressure", false },
    { 130316, "Temperature, Extended Range", false }
};

// Handle for our inter-task queue
QueueHandle_t msg_queue_nmea = NULL;

#define PGN_DIR_COUNT (sizeof(pgn_directory) / sizeof(pgn_directory[0]))

// Helper function to scan the directory for a matching label
const char* get_pgn_label(uint32_t pgn) {
    for (size_t i = 0; i < PGN_DIR_COUNT; i++) {
        if (pgn_directory[i].pgn == pgn) {
            return pgn_directory[i].label;
        }
    }
    return "Unknown / Proprietary Packet"; // Fallback label
}

void init_nmea2000_bus(void) {
    // init the message queue
    msg_queue_nmea = xQueueCreate(5, sizeof(nmea_msg_t));
    if (msg_queue_nmea == NULL) {
        ESP_LOGE(TAG, "Error creating the queue");
        return;
    }
    // 1. Establish General IO routing configs
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        CAN_TX_IO_NUM, 
        CAN_RX_IO_NUM, 
        TWAI_MODE_NORMAL
    );
    
    // 2. Enforce the strict 250 kbps NMEA 2000 baseline standard
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    
    // 3. Listen to all IDs without restrictive hardware masking
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Install and spin up the transceiver engine
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        twai_start();
        printf("NMEA 2000 CAN Receiver initialized successfully at 250kbps.\n");
    } else {
        printf("Failed to initialize CAN Hardware Bus!\n");
    }
}

// Helper function to pull the PGN out of a standard 29-bit CAN Identifier packet
uint32_t get_pgn_from_id(uint32_t id) {
    // Extracted by shifting past the source address and priority fields
    return (id >> 8) & 0x03FFFF;
}

uint8_t get_source_from_id(uint32_t id) {
    // The source address is in the lowest 8 bits (0xFF)
    return id & 0xFF;
}
/*
//create fake nmea data on queue for testing
void nmea_fake_to_queue(){
    nmea_msg_t msg;
    nmea_msg_t msg2;
    while (1) {
        //COG
        int temp = 0;
        msg.type = UI_UPDATE_COG;
        temp = rand() % 360;
        msg.data.int_val = temp;
        xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
        //ESP_LOGW("SENDERFAKE", "Sent value: %d on Core %d", temp, xPortGetCoreID());
        //SOG
        msg2.type = UI_UPDATE_SOG;
        float random_float = (rand() % 101) / 10.0f;
        msg2.data.float_val = random_float;
        xQueueSend(msg_queue_nmea, &msg2, portMAX_DELAY);
        //ESP_LOGW("SENDERFAKE", "Sent value: %d on Core %d", random_float, xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}*/
void nmea_process_to_queue(){
    twai_message_t message;
    nmea_msg_t msg;

    while (1) {
        // Wait indefinitely or until an incoming frame lands in the buffer queue
        if (twai_receive(&message, portMAX_DELAY) == ESP_OK) {
            // NMEA 2000 strictly utilizes Extended 29-bit frames
            if (message.extd) {
                uint32_t pgn = get_pgn_from_id(message.identifier);
                uint8_t src = get_source_from_id(message.identifier);
                // Parse standard marine data streams based on their PGN type
                switch(pgn) {
                    case 127505: { // Fluid Level PGN
                        // Fluid Type sits in the first 4 bits of Byte 0
                        uint8_t fluid_type = message.data[0] & 0x0F;
                        // Fluid Instance sits in the last 4 bits of Byte 0 (e.g., Tank 0, Tank 1)
                        uint8_t instance   = (message.data[0] >> 4) & 0x0F;
                        
                        // Fluid Level sits in bytes 1-2 (16-bit unsigned, resolution 0.004%)
                        uint16_t raw_level = ((uint16_t)message.data[2] << 8) | message.data[1];
                        // Tank Capacity sits in bytes 3-6 (32-bit unsigned, resolution 0.1 L)
                        uint32_t raw_cap   = ((uint32_t)message.data[6] << 24) |
                                             ((uint32_t)message.data[5] << 16) |
                                             ((uint32_t)message.data[4] << 8)  |
                                             message.data[3];

                        // We only care if the fluid type is Fuel (Fuel Type ID = 0)
                        if (fluid_type == 0) {
                            float fuel_level_percent = 0.0f;
                            float tank_capacity_liters = 0.0f;
                            float fuel_level_liters = 0.0f;

                            // 1. Process Fuel Level Percentage (0xFFFF is unavailable)
                            if (raw_level != 0xFFFF) {
                                fuel_level_percent = raw_level * 0.004f;
                            }

                            // 2. Process Tank Capacity (0xFFFFFFFF is unavailable)
                            if (raw_cap != 0xFFFFFFFF) {
                                tank_capacity_liters = raw_cap * 0.1f;
                            }

                            // 3. Dispatch to your UI update queue
                            if (raw_level != 0xFFFF) {
                                fuel_level_liters = (fuel_level_percent / 100.0f) * tank_capacity_liters;
                                msg.type = UI_UPDATE_FUELCAPACITY;
                                msg.data.float_val = tank_capacity_liters;
                                xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);

                                msg.type = UI_UPDATE_FUELPERCENT;
                                msg.data.float_val = fuel_level_percent;
                                xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);

                                msg.type = UI_UPDATE_FUELLEVEL;
                                msg.data.float_val = fuel_level_liters;
                                xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);

                                //printf("[Source: %d] Fuel Tank %d: %.1f%% (Cap: %.1fL)\n", 
                                //       src, instance, fuel_level_percent, tank_capacity_liters);
                            }
                        }
                        break;
                    }
                    case 129026: { // COG & SOG, Rapid Update PGN
                        // COG sits in bytes 2-3 (16-bit unsigned, resolution 1x10^-4 rad)
                        uint16_t raw_cog = ((uint16_t)message.data[3] << 8) | message.data[2];
                        // SOG sits in bytes 4-5 (16-bit unsigned, resolution 1x10^-2 m/s)
                        uint16_t raw_sog = ((uint16_t)message.data[5] << 8) | message.data[4];

                        // Process Course Over Ground (if available)
                        if (raw_cog != 0xFFFF) {
                            float cog_rad = raw_cog * 0.0001f;
                            float cog_deg = cog_rad * (180.0f / 3.14159265f); // Convert to degrees if needed

                            msg.type = UI_UPDATE_COG;
                            msg.data.float_val = cog_deg; 
                            xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                        }

                        // Process Speed Over Ground (if available)
                        if (raw_sog != 0xFFFF) {
                            float sog_ms = raw_sog * 0.01f;
                            float sog_knots = sog_ms * 1.94384f; // Convert m/s to knots for marine display

                            msg.type = UI_UPDATE_SOG;
                            msg.data.float_val = sog_knots; 
                            xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);

                            //printf("[Source: %d] SOG: %.1f kts\n", src, sog_knots);
                        }
                        break;
                    }
                    case 130306: { // Wind Data PGN
                        // Wind Speed sits in bytes 1-2 (16-bit unsigned, resolution 0.01 m/s)
                        uint16_t raw_speed = ((uint16_t)message.data[2] << 8) | message.data[1];
                        // Wind Direction sits in bytes 3-4 (16-bit unsigned, resolution 0.0001 rad)
                        uint16_t raw_dir   = ((uint16_t)message.data[4] << 8) | message.data[3];
                        // Wind Reference sits in bits 0-2 of byte 5
                        uint8_t reference  = message.data[5] & 0x07; 

                        float wind_speed_knots = 0.0f;
                        float wind_dir_deg = 0.0f;

                        // 1. Process Wind Speed (if available)
                        if (raw_speed != 0xFFFF) {
                            float speed_ms = raw_speed * 0.01f;
                            wind_speed_knots = speed_ms * 1.94384f; // Convert m/s to knots
                        }

                        // 2. Process Wind Direction (if available)
                        if (raw_dir != 0xFFFF) {
                            float dir_rad = raw_dir * 0.0001f;
                            wind_dir_deg = dir_rad * (180.0f / 3.14159265f); // Convert to degrees
                        }

                        // 3. Route to the correct UI target based on the reference type
                        if (raw_speed != 0xFFFF || raw_dir != 0xFFFF) {
                            switch (reference) {
                                case 0: // Apparent Wind
                                    msg.type = UI_UPDATE_AWS;
                                    msg.data.float_val = wind_speed_knots;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    msg.type = UI_UPDATE_AWD;
                                    msg.data.float_val = wind_dir_deg;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;

                                case  1: // True Wind (referenced to boat heading/bow)
                                case 2: // True Wind (referenced to true North / ground)
                                case 3: // True Wind (referenced to magnetic North)
                                    msg.type = UI_UPDATE_TWS;
                                    msg.data.float_val = wind_speed_knots;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    msg.type = UI_UPDATE_TWD;
                                    msg.data.float_val = wind_dir_deg;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;
                                default:
                                    // Unknown or reserved reference type
                                    break;
                            }
                        }
                        break;
                    }
                    case 128267: { // Water Depth PGN
                        // Distance payload sits inside bytes 1-4 (Byte 0 is SID)
                        uint32_t raw_depth = ((uint32_t)message.data[4] << 24) | 
                                             ((uint32_t)message.data[3] << 16) | 
                                             ((uint32_t)message.data[2] << 8)  | 
                                             message.data[1];
                        if (raw_depth == 0xFFFFFFFF) {
                            // depth unavailable
                             break; 
                        } else {
                            float actual_depth_m = (raw_depth * 0.01) + DEPTH_OFFSET; 
                            //fill struct and place on queue
                            msg.type = UI_UPDATE_DEPTH;
                            msg.data.float_val = actual_depth_m;
                            xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                        }
                        break;
                    }
                    case 130316: { // Temperature, Extended Range PGN
                        uint8_t instance = message.data[1];
                        uint8_t source   = message.data[2];
                        // Extract the 24-bit temperature field from bytes 3, 4, and 5
                        uint32_t raw_24bits = ((uint32_t)message.data[5] << 16) | 
                                              ((uint32_t)message.data[4] << 8)  | 
                                              message.data[3];
                        // Missing data marker for a 24-bit integer is 0x00FFFFFF
                        if (raw_24bits != 0x00FFFFFF) {
                            // Sign-extend the 24-bit value to a standard 32-bit signed integer
                            int32_t signed_temp = (int32_t)raw_24bits;
                            if (signed_temp & 0x00800000) { 
                                signed_temp |= 0xFF000000; // Extend the negative sign bit
                            }
                            // 130316 resolution is 0.001 Kelvin
                            float temp_kelvin = signed_temp * 0.001f;
                            float temp_celsius = temp_kelvin - 273.15f;

                            switch(source) {
                                case 1: {
                                    msg.type = UI_UPDATE_TEMP_OUTSIDE;
                                    msg.data.float_val = temp_celsius;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;
                                }
                                case 2: {
                                    msg.type = UI_UPDATE_TEMP_INSIDE;
                                    msg.data.float_val = temp_celsius;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;
                                }
                                default:
                                    break;
                            }
                            //printf("[Source: %d] Temp Source %d (Inst %d): %.2f°C\n", src, source, instance, temp_celsius);
                        }
                        break;
                    }
                    case 130313: { // Humidity PGN
                        uint8_t instance = message.data[1];
                        uint8_t source   = message.data[2];
                        // Humidity sits in bytes 3-4 (16-bit unsigned, resolution 0.004 %)
                        uint16_t raw_humidity = ((uint16_t)message.data[4] << 8) | message.data[3];

                        if (raw_humidity != 0xFFFF) {
                            float humidity_percent = raw_humidity * 0.004f;

                            switch(source) {
                                case 1: {
                                    msg.type = UI_UPDATE_HUM_OUTSIDE;
                                    msg.data.float_val = humidity_percent;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;
                                }
                                case 0: {
                                    msg.type = UI_UPDATE_HUM_INSIDE;
                                    msg.data.float_val = humidity_percent;
                                    xQueueSend(msg_queue_nmea, &msg, portMAX_DELAY);
                                    break;
                                }
                                default:
                                    break;
                            }
                            //msg.type = UI_UPDATE_HUMIDITY;
                            //msg.data.environment.instance = instance;
                            //msg.data.environment.source = source;
                            //msg.data.environment.value = humidity_percent;

                            //printf("[Source: %d] Humidity Source %d (Inst %d): %.1f%%\n", src, source, instance, humidity_percent);
                        }
                        break;
                    }
                    default:
                        // Catch-all monitor to trace unmapped traffic packets
                        // printf("Caught PGN: %-25s | Size: %d Bytes\n", get_pgn_label(pgn), message.data_length_code);
                        break;
                }
            }
        }
    }
}