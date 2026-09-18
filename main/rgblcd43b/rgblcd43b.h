#ifndef RGBLCD43B_H
#define RGBLCD43B_H

#include <stdio.h>
#include "esp_check.h"        // Dependent header file
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

//define the pins and other configs for the RGB LCD panel in the Waveshare4.3b
#define RGB_LCD_DATA_WIDTH 16
#define RGB_LCD_BIT_PER_PIXEL 16
//#define RGB_LCD_NUM_FB 1
#define RGB_LCD_H_RES 800 //horizontal resolution
#define RGB_LCD_V_RES 480 //vertical resolution

#define RGB_PIN_NUM_DISP_EN -1//EXIO2
#define RGB_PIN_NUM_PCLK 7
#define RGB_PIN_NUM_VSYNC 3
#define RGB_PIN_NUM_HSYNC 46
#define RGB_PIN_NUM_DE 5 

#define RGB_LCD_PIXEL_CLOCK_HZ      (16 * 1000 * 1000)

#define RGB_PIN_NUM_DATA0 14 //B3
#define RGB_PIN_NUM_DATA1 38 //B4
#define RGB_PIN_NUM_DATA2 18 //B5
#define RGB_PIN_NUM_DATA3 17 //B6
#define RGB_PIN_NUM_DATA4 10 //B7
#define RGB_PIN_NUM_DATA5 39 //G2
#define RGB_PIN_NUM_DATA6 0 //G3
#define RGB_PIN_NUM_DATA7 45 //G4
#define RGB_PIN_NUM_DATA8 48 //G5
#define RGB_PIN_NUM_DATA9 47 //G6
#define RGB_PIN_NUM_DATA10 21//G7
#define RGB_PIN_NUM_DATA11 1//R3
#define RGB_PIN_NUM_DATA12 2//R4
#define RGB_PIN_NUM_DATA13 42//R5
#define RGB_PIN_NUM_DATA14 41//R6
#define RGB_PIN_NUM_DATA15 40//R7

esp_err_t rgblcd_panel_init(uint8_t frame_buffer_count, esp_lcd_panel_handle_t *out_panel_handle);

#endif //RGBLCD43B_H