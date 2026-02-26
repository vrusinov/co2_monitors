// SPDX-FileCopyrightText: 2024 Vladimir Rusinov
// SPDX-License-Identifier: Apache-2.0

#ifndef NETATMO_ESPHOME_M5STICKC_DISPLAY_NETATMO_DISPLAY_H_
#define NETATMO_ESPHOME_M5STICKC_DISPLAY_NETATMO_DISPLAY_H_

#include <cstdio>

#include "esphome.h" // NOLINT(build/include_subdir)

uint8_t component_value(double v, int v_min, int v_range,
                        uint8_t max_color_value, uint8_t min_color_value) {
  int color_range = max_color_value - min_color_value;
  if (color_range == 0) {
    return min_color_value;
  } else {
    return uint8_t(min_color_value + color_range * (v - v_min) / v_range);
  }
}

Color gradient_color(double v, int min, Color min_color, int max,
                     Color max_color) {
  if (v <= min) {
    return min_color;
  }
  if (v >= max) {
    return max_color;
  }

  int range = max - min;

  uint8_t red_value =
      component_value(v, min, range, max_color.red, min_color.red);
  uint8_t green_value =
      component_value(v, min, range, max_color.green, min_color.green);
  uint8_t blue_value =
      component_value(v, min, range, max_color.blue, min_color.blue);

  return Color(red_value, green_value, blue_value);
}

void display_update(DisplayBuffer &it) {
  auto double_gradient = [](double v, int min, Color min_color, int mid,
                            Color mid_color, int max, Color max_color) {
    if (v <= min) {
      return min_color;
    } else if (v <= mid) {
      return gradient_color(v, min, min_color, mid, mid_color);
    } else if (v < max) {
      return gradient_color(v, mid, mid_color, max, max_color);
    } else {
      return max_color;
    }
  };

  auto color_red = Color(255, 0, 0);
  auto color_green = Color(0, 255, 0);
  auto color_blue = Color(0, 0, 255);
  auto color_white = Color(255, 255, 255);

  // Cabinet ppm - display in the center.
  if (id(cabinet_co2).has_state()) {
    int cabinet_ppm = id(cabinet_co2).state;
    it.printf(it.get_width() / 2, it.get_height() / 2, id(large_font),
              double_gradient(static_cast<double>(cabinet_ppm), 300,
                              color_green, 700, color_white, 2000, color_red),
              TextAlign::CENTER, "%d", cabinet_ppm);
  }

  // averages
  float avg_temp = 0;
  int avg_ppm = 0;
  if (id(cabinet_temp).has_state() && id(living_room_temp).has_state() &&
      id(bedroom_temp).has_state()) {
    avg_temp = (id(cabinet_temp).state + id(living_room_temp).state +
                id(bedroom_temp).state) /
               3;
  }
  if (id(cabinet_co2).has_state() && id(living_room_co2).has_state() &&
      id(bedroom_co2).has_state()) {
    avg_ppm = (id(cabinet_co2).state + id(living_room_co2).state +
               id(bedroom_co2).state) /
              3;
  }
  if (avg_temp || avg_ppm) {
    char buf[128] = "In: ";
    int x_start, y_start;
    int width, height;
    auto font = id(font3);
    int h = 0;
    auto align = TextAlign::TOP_LEFT;

    it.print(0, h, font, align, buf);
    it.get_text_bounds(0, h, buf, font, align, &x_start, &y_start, &width,
                       &height);

    if (avg_temp) {
      snprintf(buf, sizeof(buf), "%.1f°C ", avg_temp);
      it.print(x_start + width, h, font,
               double_gradient(avg_temp, 15, color_blue, 23, color_white, 25,
                               color_red),
               align, buf);
      it.get_text_bounds(x_start + width, h, buf, font, align, &x_start,
                         &y_start, &width, &height);
    }
    if (avg_ppm) {
      snprintf(buf, sizeof(buf), "%d ppm", avg_ppm);
      it.print(x_start + width, h, font,
               double_gradient(static_cast<double>(avg_ppm), 300, color_green,
                               700, color_white, 2000, color_red),
               align, buf);
      it.get_text_bounds(x_start + width, h, buf, font, align, &x_start,
                         &y_start, &width, &height);
    }
  }

  // Outside temperature.
  if (id(outdoor_temp).has_state()) {
    int x_start, y_start;
    int width, height;
    char buf[128] = "Out: ";

    double t = id(outdoor_temp).state;

    it.print(0, it.get_height(), id(font3), TextAlign::BOTTOM_LEFT, buf);
    it.get_text_bounds(0, it.get_height(), buf, id(font3),
                       TextAlign::BOTTOM_LEFT, &x_start, &y_start, &width,
                       &height);

    it.printf(
        width, it.get_height(), id(font3),
        double_gradient(t, -5, color_blue, 15, color_white, 30, color_red),
        TextAlign::BOTTOM_LEFT, "%.1f°C", t);
  }
}

#endif // NETATMO_ESPHOME_M5STICKC_DISPLAY_NETATMO_DISPLAY_H_
