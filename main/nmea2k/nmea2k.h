#ifndef NMEA2K_H
#define NMEA2K_H

#include <stdio.h>
#include <stdbool.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdio.h>
#include "driver/twai.h"

// Waveshare 4.3" Native Hardware CAN Pins
#define CAN_TX_IO_NUM       15
#define CAN_RX_IO_NUM       16
#define DEPTH_OFFSET        0.4

extern QueueHandle_t msg_queue_nmea;

typedef enum {
    UI_UPDATE_NULL,
    UI_UPDATE_LATITUDE,
    UI_UPDATE_COG,
    UI_UPDATE_SOG,
    UI_UPDATE_LONGITUDE,
    UI_UPDATE_DEPTH,
    UI_UPDATE_SATELLITES
} ui_update_type_t;

typedef struct {
    ui_update_type_t type;
    union {
        float float_val;
        int int_val;
        char str_val[16]; // For pre-formatted strings
    } data;
} nmea_msg_t;

const char* get_pgn_label(uint32_t pgn);
void init_nmea2000_bus(void);
uint32_t get_pgn_from_id(uint32_t id);
uint8_t get_source_from_id(uint32_t id);
void nmea_fake_to_queue();
void nmea_process_to_queue();


#endif // NMEA2K_H