#include "esphome.h"

uint8_t component_value(double v, int v_min, int v_range, uint8_t max_color_value, uint8_t min_color_value) {
    int color_range = max_color_value - min_color_value;
    if (color_range == 0) {
        return min_color_value;
    } else {
        return uint8_t(min_color_value + color_range * (v - v_min) / v_range);
    }
}

Color gradient_color(double v, int min, Color min_color, int max, Color max_color) {
    if (v <= min) {
        return min_color;
    }
    if (v >= max) {
        return max_color;
    }

    int range = max - min;

    uint8_t red_value = component_value(v, min, range, max_color.red, min_color.red);
    uint8_t green_value = component_value(v, min, range, max_color.green, min_color.green);
    uint8_t blue_value = component_value(v, min, range, max_color.blue, min_color.blue);

    return Color(red_value, green_value, blue_value);
};