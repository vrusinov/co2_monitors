#include "esphome.h"

uint8_t component_value(double v, int v_min, int v_range, uint8_t max_color_value, uint8_t min_color_value) {
    int color_range = max_color_value - min_color_value;
    if (color_range == 0) {
        return min_color_value;
    } else {
        return uint8_t(min_color_value + color_range * (v - v_min) / v_range);
    }
}