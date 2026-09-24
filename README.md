# ESP32BoatAssistant
A Boat assistant project using the Waveshare ESP32S3 4.3B Box hardware


[app_main Startup]
  │── 1. Init Hardware (SPI, I2C, TWAI, Wi-Fi)
  │── 2. Init LVGL Base & Alloc Buffers
  │── 3. Create Mutexes & Queues
  └── 4. Spawn Tasks ──┐
                       │
         ┌─────────────┴────────────────────────┐
         ▼                                      ▼
  [CORE 1: UI & Input]                  [CORE 0: I/O & Network]
   ├── Task: LVGL Engine                 ├── Task: TWAI Receiver
   │    └── lv_timer_handler()           │    └── twai_receive()
   └── Task: Touch Interrupt Handler     └── Task: REST API Client
        └── Reads I2C touch coords            └── HTTP Request/cJSON
