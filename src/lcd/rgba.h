/**
 * File:   rgba.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief: rgba
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

typedef uint32_t pixel_t;

#define LCD_FORMAT BITMAP_FMT_RGBA

#define rgb_to_pixel(r, g, b) ((r) << 24) | ((g) << 16) | ((b) << 8) | 0xff
static inline pixel_t to_pixel(color_t c) { return rgb_to_pixel(c.rgba.r, c.rgba.g, c.rgba.b); }

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  uint8_t minus_a = 0xff - a;
  uint16_t rr = (bg.rgba.r * minus_a + fg.rgba.r * a) / 0xff;
  uint16_t gg = (bg.rgba.g * minus_a + fg.rgba.g * a) / 0xff;
  uint16_t bb = (bg.rgba.b * minus_a + fg.rgba.b * a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}

static inline color_t to_color(pixel_t pixel) {
  color_t c;
  c.rgba.r = (pixel >> 24) & 0xff;
  c.rgba.g = (pixel >> 16) & 0xff;
  c.rgba.b = (pixel >> 8) & 0xff;
  c.rgba.a = pixel & 0xff;

  return c;
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  uint16_t rr = ((0xff & (pixel >> 24)) * minus_a + c.rgba.r * a) / 0xff;
  uint16_t gg = ((0xff & (pixel >> 16)) * minus_a + c.rgba.g * a) / 0xff;
  uint16_t bb = ((0xff & (pixel >> 8)) * minus_a + c.rgba.b * a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  uint16_t rr = (fg.rgba.r + a) / 0xff;
  uint16_t gg = (fg.rgba.g + a) / 0xff;
  uint16_t bb = (fg.rgba.b + a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}
