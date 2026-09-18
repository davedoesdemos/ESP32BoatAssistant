#include "rgblcd43b.h"

//logging
static const char *TAG = "boat assistant rgblcd43b";

//https://github.com/waveshareteam/ESP32-S3-Touch-LCD-4.3B/blob/master/examples/ESP-IDF/09_lvgl_v9_demo/components/waveshare_rgb_lcd_port.c
esp_err_t rgblcd_panel_init(uint8_t frame_buffer_count, esp_lcd_panel_handle_t *out_panel_handle)
{
    ESP_RETURN_ON_FALSE(out_panel_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "Panel handle pointer cannot be NULL");

 esp_lcd_rgb_panel_config_t panel_config = {   // Configuration parameters for the RGB interface
    .data_width = RGB_LCD_DATA_WIDTH,               // Data line width of the RGB interface, e.g., `16-bit RGB565`: 16, `8-bit RGB888`: 8
    .bits_per_pixel = RGB_LCD_BIT_PER_PIXEL,        // Number of bits for the color format, may not be equal to the data line width of the RGB interface,
                                                        // e.g., `16-bit RGB565`: 16, `8-bit RGB888`: 24
    .psram_trans_align = 64,                            // Set to `64` by default
    .num_fbs = frame_buffer_count,                      // Number of frame buffers for the RGB interface, set to `1` by default, greater than `1` for multiple buffering to prevent tearing
    .bounce_buffer_size_px = 10 * RGB_LCD_H_RES,    // Used to increase the data transfer bandwidth of the RGB interface, usually set to `10 * EXAMPLE_LCD_H_RES`
    .clk_src = LCD_CLK_SRC_DEFAULT,                     // Set to `LCD_CLK_SRC_DEFAULT` by default
    .disp_gpio_num = RGB_PIN_NUM_DISP_EN,           // Pin number connected to the LCD DISP signal, can be set to `-1` to disable
    .pclk_gpio_num = RGB_PIN_NUM_PCLK,              // Pin number connected to the LCD PCLK signal
    .vsync_gpio_num = RGB_PIN_NUM_VSYNC,            // Pin number connected to the LCD VSYNC signal
    .hsync_gpio_num = RGB_PIN_NUM_HSYNC,            // Pin number connected to the LCD HSYNC signal
    .de_gpio_num = RGB_PIN_NUM_DE,                  // Pin number connected to the LCD DE signal, can be set to `-1` to disable
    .data_gpio_nums = {                                 // Pin numbers connected to the LCD D[15:0] signals, the valid quantity is specified by `data_width`,
                                                        // set to D[7:0] for 8-bit
        RGB_PIN_NUM_DATA0, //B1
        RGB_PIN_NUM_DATA1, //B2
        RGB_PIN_NUM_DATA2, //B3
        RGB_PIN_NUM_DATA3, //B4
        RGB_PIN_NUM_DATA4, //B5
        RGB_PIN_NUM_DATA5, //G1
        RGB_PIN_NUM_DATA6, //G2
        RGB_PIN_NUM_DATA7, //G3
        RGB_PIN_NUM_DATA8, //G4
        RGB_PIN_NUM_DATA9, //G5
        RGB_PIN_NUM_DATA10, //G6
        RGB_PIN_NUM_DATA11, //R1
        RGB_PIN_NUM_DATA12, //R2
        RGB_PIN_NUM_DATA13, //R3
        RGB_PIN_NUM_DATA14, //R4
        RGB_PIN_NUM_DATA15, //R5
    },
    .timings = {        // The following are parameters related to RGB timing, which need to be determined based on the datasheet of the LCD driver IC and hardware configuration
        .pclk_hz = RGB_LCD_PIXEL_CLOCK_HZ,
        .h_res = RGB_LCD_H_RES,
        .v_res = RGB_LCD_V_RES,
        .hsync_back_porch = 8,         // In DE mode, parameters related to HSYNC and VSYNC can be adjusted according to the desired refresh rate
        .hsync_front_porch = 8,        // In SYNC mode, parameters related to HSYNC and VSYNC need to be consistent with the configuration in the software initialization command
        .hsync_pulse_width = 4,
        .vsync_back_porch = 8,
        .vsync_front_porch = 8,
        .vsync_pulse_width = 4,
        .flags = {      // Since some LCDs can configure these parameters through hardware pins, make sure they are consistent with the configuration, but usually set to `0`
          .hsync_idle_low = 0,    // Level when the HSYNC signal is idle, 0: high level, 1: low level
          .vsync_idle_low = 0,    // Level when the VSYNC signal is idle, 0: high level, 1: low level
          .de_idle_high = 0,      // Level when the DE signal is idle, 0: high level, 1: low level
          .pclk_active_neg = 1,   // Effective edge of the clock signal, 0: rising edge, 1: falling edge
          .pclk_idle_high = 0,    // Level when the PCLK signal is idle, 0: high level, 1: low level
        },
    },
    .flags.fb_in_psram = 1,       // Set to `1` by default
};

    // Allocate and initialize the driver
    ESP_RETURN_ON_ERROR(esp_lcd_new_rgb_panel(&panel_config, out_panel_handle), TAG, "Failed to create RGB panel");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(*out_panel_handle), TAG, "Failed to reset panel");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(*out_panel_handle), TAG, "Failed to init panel");

    return ESP_OK;
}