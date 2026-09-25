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
}
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
                    case 127488: { // Engine Speed / RPM PGN
                        // RPM data is inside bytes 2 and 3 (0-indexed)
                        uint16_t raw_rpm = (message.data[3] << 8) | message.data[2];
                        float actual_rpm = raw_rpm * 0.25; // NMEA multiplier scale
                        printf("NMEA 2000 Data -> Engine RPM: %.2f\n", actual_rpm);
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

                            //old print statement, remove after testing
                            printf("[Source: %d] Water Depth: %.2fm\n", src, actual_depth_m);
                        }
                        break;              
                        /* float actual_depth_m = raw_depth * 0.01; // Scale factor in meters
                        // printf("NMEA 2000 Data -> Water Depth: %.2fm\n", actual_depth_m);
                        printf("[Source: %d] Water Depth: %.2fm\n", src, actual_depth_m);
                        break; */
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