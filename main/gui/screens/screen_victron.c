#include "screen_victron.h"

void screen_victron_layout(lv_obj_t *screen_victron) {
    // Layout row for three columns
    lv_obj_t * row1_container = lv_obj_create(screen_victron);
    lv_obj_remove_style_all(row1_container);
    lv_obj_set_height(row1_container, 450);
    lv_obj_set_width(row1_container, lv_pct(100));
    lv_obj_set_y( row1_container, 28 );
    lv_obj_set_flex_flow(row1_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        // column 0
        lv_obj_t * column0_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column0_container);
        lv_obj_set_layout(column0_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column0_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column0_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_height(column0_container, lv_pct(100));
        lv_obj_set_width(column0_container, lv_pct(25));

            // MPPT Label
            lv_obj_t *mppt1_label = lv_label_create(column0_container);
            lv_obj_set_style_text_color(mppt1_label, lv_color_hex(0x000000), 0);
            lv_label_set_text(mppt1_label, "MPPT 1");
            lv_obj_set_style_text_align(mppt1_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(mppt1_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(mppt1_label, &lv_font_montserrat_14, 0); 

            // MPPT 1 Arc
            lv_obj_t *mppt1_arc1 = lv_arc_create(column0_container);
            lv_obj_set_size(mppt1_arc1, 180, 180);
            lv_obj_set_style_pad_all(mppt1_arc1, 0, LV_PART_MAIN);
            lv_arc_set_rotation(mppt1_arc1, 135);
            lv_arc_set_bg_angles(mppt1_arc1, 0, 270);
            lv_arc_set_range(mppt1_arc1, 0, 200);
            // Style
            lv_obj_set_style_arc_width(mppt1_arc1, 20, 0);
            lv_obj_set_style_arc_width(mppt1_arc1, 20, LV_PART_INDICATOR);
            lv_obj_set_style_arc_color(mppt1_arc1, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(mppt1_arc1, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_remove_flag(mppt1_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

            lv_arc_bind_value(mppt1_arc1, state_victron_mppt1_power);

            // MPPT 1 Power Label
            lv_obj_t *mppt1_power_label = lv_label_create(mppt1_arc1);
            lv_obj_set_style_text_color(mppt1_power_label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_align(mppt1_power_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(mppt1_power_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(mppt1_power_label, &lv_font_montserrat_24, 0);

            lv_label_bind_text(mppt1_power_label, state_victron_mppt1_power_text, NULL);

            // MPPT 2 Label
            lv_obj_t *mppt2_label = lv_label_create(column0_container);
            lv_obj_set_style_text_color(mppt2_label, lv_color_hex(0x000000), 0);
            lv_label_set_text(mppt2_label, "MPPT 2");
            lv_obj_set_style_text_align(mppt2_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(mppt2_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(mppt2_label, &lv_font_montserrat_14, 0); 

            // MPPT 2 Arc
            lv_obj_t *mppt2_arc1 = lv_arc_create(column0_container);
            lv_obj_set_size(mppt2_arc1, 180, 180);
            lv_obj_set_style_pad_all(mppt2_arc1, 0, LV_PART_MAIN);
            lv_arc_set_rotation(mppt2_arc1, 135);
            lv_arc_set_bg_angles(mppt2_arc1, 0, 270);
            lv_arc_set_range(mppt2_arc1, 0, 200);
            // Style
            lv_obj_set_style_arc_width(mppt2_arc1, 20, 0);
            lv_obj_set_style_arc_width(mppt2_arc1, 20, LV_PART_INDICATOR);
            lv_obj_set_style_arc_color(mppt2_arc1, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(mppt2_arc1, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_remove_flag(mppt2_arc1, LV_OBJ_FLAG_CLICKABLE);

            lv_arc_bind_value(mppt2_arc1, state_victron_mppt2_power);

            // MPPT 2 Power Label
            lv_obj_t *mppt2_power_label = lv_label_create(mppt2_arc1);
            lv_obj_set_style_text_color(mppt2_power_label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_align(mppt2_power_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(mppt2_power_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(mppt2_power_label, &lv_font_montserrat_24, 0);

            lv_label_bind_text(mppt2_power_label, state_victron_mppt2_power_text, NULL);

        // column 1
        lv_obj_t * column1_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column1_container);
        lv_obj_set_layout(column1_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column1_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column1_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_height(column1_container, lv_pct(100));
        lv_obj_set_width(column1_container, lv_pct(50));

            // House Battery Label
            lv_obj_t *house_battery_label = lv_label_create(column1_container);
            lv_obj_set_style_text_color(house_battery_label, lv_color_hex(0x000000), 0);
            lv_label_set_text(house_battery_label, "House Battery");
            lv_obj_set_style_text_align(house_battery_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(house_battery_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(house_battery_label, &lv_font_montserrat_14, 0); 

            // House Battery Arc
            lv_obj_t *house_battery_arc1 = lv_arc_create(column1_container);
            lv_obj_set_size(house_battery_arc1, 380, 380);
            lv_obj_set_style_pad_all(house_battery_arc1, 0, LV_PART_MAIN);
            lv_arc_set_rotation(house_battery_arc1, 135);
            lv_arc_set_bg_angles(house_battery_arc1, 0, 270);
            lv_arc_set_range(house_battery_arc1, 0, 100);
            // Style
            lv_obj_set_style_arc_width(house_battery_arc1, 30, 0);
            lv_obj_set_style_arc_width(house_battery_arc1, 30, LV_PART_INDICATOR);
            lv_obj_set_style_arc_color(house_battery_arc1, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(house_battery_arc1, lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_remove_flag(house_battery_arc1, LV_OBJ_FLAG_CLICKABLE);

            lv_arc_bind_value(house_battery_arc1, state_victron_house_battery_soc);

            // House Battery SOC
            lv_obj_t *house_battery_soc_label = lv_label_create(house_battery_arc1);
            lv_obj_set_style_text_color(house_battery_soc_label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_align(house_battery_soc_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(house_battery_soc_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(house_battery_soc_label, &lv_font_montserrat_24, 0);

            lv_label_bind_text(house_battery_soc_label, state_victron_house_battery_soc_text, NULL);

        // column 2
        lv_obj_t * column2_container = lv_obj_create(row1_container);
        lv_obj_remove_style_all(column2_container);
        lv_obj_set_layout(column2_container, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(column2_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(column2_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_height(column2_container, lv_pct(100));
        lv_obj_set_width(column2_container, lv_pct(25));

            // DC2DC Label
            lv_obj_t *dc2dc_label = lv_label_create(column2_container);
            lv_obj_set_style_text_color(dc2dc_label, lv_color_hex(0x000000), 0);
            lv_label_set_text(dc2dc_label, "Orion XS");
            lv_obj_set_style_text_align(dc2dc_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(dc2dc_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(dc2dc_label, &lv_font_montserrat_14, 0); 
            // DC2DC Arc
            lv_obj_t *dc2dc_arc1 = lv_arc_create(column2_container);
            lv_obj_set_size(dc2dc_arc1, 180, 180);
            lv_obj_set_style_pad_all(dc2dc_arc1, 0, LV_PART_MAIN);
            lv_arc_set_rotation(dc2dc_arc1, 135);
            lv_arc_set_bg_angles(dc2dc_arc1, 0, 270);
            lv_arc_set_range(dc2dc_arc1, 0, 700);
            // Style
            lv_obj_set_style_arc_width(dc2dc_arc1, 20, 0);
            lv_obj_set_style_arc_width(dc2dc_arc1, 20, LV_PART_INDICATOR);
            lv_obj_set_style_arc_color(dc2dc_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(dc2dc_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_remove_flag(dc2dc_arc1, LV_OBJ_FLAG_CLICKABLE); // Make it read-only (disable dragging)

            lv_arc_bind_value(dc2dc_arc1, state_victron_dc2dc_power);

            // Centred Text Labels for values
            lv_obj_t *dc2dc_power_label = lv_label_create(dc2dc_arc1);
            lv_obj_set_style_text_color(dc2dc_power_label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_align(dc2dc_power_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(dc2dc_power_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(dc2dc_power_label, &lv_font_montserrat_24, 0);

            lv_label_bind_text(dc2dc_power_label, state_victron_dc2dc_power_text, NULL);

            // Charger Label
            lv_obj_t *charger_label = lv_label_create(column2_container);
            lv_obj_set_style_text_color(charger_label, lv_color_hex(0x000000), 0);
            lv_label_set_text(charger_label, "Charger");
            lv_obj_set_style_text_align(charger_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(charger_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(charger_label, &lv_font_montserrat_14, 0); 

            // Charger Arc
            lv_obj_t *charger_arc1 = lv_arc_create(column2_container);
            lv_obj_set_size(charger_arc1, 180, 180);
            lv_obj_set_style_pad_all(charger_arc1, 0, LV_PART_MAIN);
            lv_arc_set_rotation(charger_arc1, 135);
            lv_arc_set_bg_angles(charger_arc1, 0, 270);
            lv_arc_set_range(charger_arc1, 0, 700);
            // Style
            lv_obj_set_style_arc_width(charger_arc1, 20, 0);
            lv_obj_set_style_arc_width(charger_arc1, 20, LV_PART_INDICATOR);
            lv_obj_set_style_arc_color(charger_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(charger_arc1, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_remove_flag(charger_arc1, LV_OBJ_FLAG_CLICKABLE);

            lv_arc_bind_value(charger_arc1, state_victron_charger_power);

            // Centred Text Labels for values
            lv_obj_t *charger_power_label = lv_label_create(charger_arc1);
            lv_obj_set_style_text_color(charger_power_label, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_text_align(charger_power_label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_align(charger_power_label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(charger_power_label, &lv_font_montserrat_24, 0);

            lv_label_bind_text(charger_power_label, state_victron_charger_power_text, NULL);
}