idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES rgblcd43b esp_lcd lvgl i2c esp_lcd_touch esp_lcd_touch_gt911 esp_driver_i2c expander backlight gui esp_common)
