#include "nmea2k.h"

// 1. Define a structural format for our PGN directory
typedef struct {
    uint32_t pgn;
    const char *label;
    bool is_fast_packet; // Flags if the message splits across multiple CAN frames
} n2k_pgn_meta_t;

// 2. Populate the structural database using the standardized CANboat definitions
static const n2k_pgn_meta_t pgn_directory[] = {
    { 59392,  "ISO Acknowledgment", false },
    { 60928,  "ISO Address Claim", false },
    { 126992, "System Time", false },
    { 126996, "Product Information", true },
    { 127245, "Rudder Angle", false },
    { 127250, "Vessel Heading", false },
    { 127251, "Rate of Turn", false },
    { 127257, "Attitude / Roll-Pitch", false },
    { 127488, "Engine Speed / RPM", false },
    { 127489, "Engine Dynamic Parameters", true },
    { 127505, "Fluid Level Status", false },
    { 127508, "Battery Status", false },
    { 128259, "Speed: Water Referenced", false },
    { 128267, "Water Depth", false },
    { 129025, "Position: Rapid Update", false },
    { 129026, "COG & SOG: Rapid Update", false },
    { 129029, "GNSS Position Data", true },
    { 130306, "Wind Data", false },
    { 130310, "Environmental Parameters", false },
    { 130314, "Actual Pressure", false }
};

#define PGN_DIR_COUNT (sizeof(pgn_directory) / sizeof(pgn_directory[0]))

// 3. Helper function to scan the directory for a matching label
const char* get_pgn_label(uint32_t pgn) {
    for (size_t i = 0; i < PGN_DIR_COUNT; i++) {
        if (pgn_directory[i].pgn == pgn) {
            return pgn_directory[i].label;
        }
    }
    return "Unknown / Proprietary Packet"; // Fallback label
}

void init_nmea2000_bus(void) {
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
void nmeaoldmain(){
    twai_message_t message;

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
                                   // Check if data is unavailable (0xFFFFFFFF)
                        if (raw_depth == 0xFFFFFFFF) {
                            // Option A: Print a cleaner status message
                            //printf("[Source: %d] Water Depth: DATA UNAVAILABLE\n", src);
                            
                            // Option B: Simply 'break' if you want to silently ignore it
                             break; 
                        } else {
                            float actual_depth_m = (raw_depth * 0.01) + DEPTH_OFFSET; 
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