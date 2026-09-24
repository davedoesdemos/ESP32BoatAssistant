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

const char* get_pgn_label(uint32_t pgn);
void init_nmea2000_bus(void);
uint32_t get_pgn_from_id(uint32_t id);
uint8_t get_source_from_id(uint32_t id);
void nmea_process_to_queue();


#endif // NMEA2K_H