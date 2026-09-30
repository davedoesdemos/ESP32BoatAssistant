#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "queue_reader.h"

//https://docs.ruuvi.com/communication/bluetooth-advertisements/data-format-5-rawv2
typedef struct {
    uint64_t tagid;
    float temperature;
    float humidity;
    float pressure;
    float battery_v;
} ruuvi_df5_t;

//https://docs.ruuvi.com/communication/bluetooth-advertisements/data-format-e1
typedef struct {
    uint64_t tagid;
    float temperature;
    float humidity;
    float pressure;
    float pm1_0; //Particulate Matter (PM) 1µm
    float pm2_5; //Particulate Matter (PM) 2.5µm
    float pm4_0; //Particulate Matter (PM) 4µm
    float pm10_0; //Particulate Matter (PM) 10µm
    uint16_t co2; //Carbon Dioxide (CO₂)
    uint16_t voc; //VOC Volatile Organic Compounds
    uint16_t nox; //NOx Nitrogen Oxides
    float luminosity;
    uint32_t seq_num;
} ruuvi_e1_t;

#define RUUVI_COMPANY_ID 0x0499

void ble_host_task(void *param);
void bluetooth_init();

#endif //BLUETOOTH_H