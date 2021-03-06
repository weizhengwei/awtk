/**
 * File:   lcd_mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  mem implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/vgcanvas.h"
#include "base/system_info.h"

static ret_t lcd_mem_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = dirty_rect;

  return RET_OK;
}

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t stroke_color = lcd->stroke_color;
  pixel_t color = to_pixel(stroke_color);
  pixel_t* p = pixels + y * width + x;

  for (i = 0; i < w; i++) {
    p[i] = color;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t stroke_color = lcd->stroke_color;
  pixel_t color = to_pixel(stroke_color);
  pixel_t* p = pixels + y * width + x;

  for (i = 0; i < h; i++) {
    *p = color;
    p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t color = lcd->stroke_color;
  pixel_t pixel = to_pixel(color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    pixel_t* p = pixels + point->y * width + point->x;
    if (color.rgba.a == 0xff) {
      p[0] = pixel;
    } else if (color.rgba.a) {
      p[0] = blend_pixel(p[0], color);
    }
  }

  return RET_OK;
}

color_t lcd_mem_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t p = pixels[y * width + x];

  return to_color(p);
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  wh_t i = 0;

  color_t c = lcd->stroke_color;
  lcd->stroke_color = lcd->fill_color;
  for (i = 0; i < h; i++) {
    lcd_mem_draw_hline(lcd, x, y + i, w);
  }
  lcd->stroke_color = c;

  return RET_OK;
}

static ret_t lcd_mem_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t width = lcd->w;
  color_t color = lcd->text_color;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t* dst_p = pixels + y * width + x;
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;
  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      uint8_t alpha = src_p[i];
      if (alpha) {
        color.rgba.a = alpha;
        dst_p[i] = blend_pixel(dst_p[i], color);
      }
    }
    src_p += glyph->w;
    dst_p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t dw = dst->w;
  wh_t dh = dst->h;
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t* dst_p = pixels + dst->y * width + dst->x;
  const color_t* data = (color_t*)img->data;

  if (src->w == dst->w && src->h == dst->h) {
    const color_t* src_p = data + img->w * src->y + src->x;
    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        color_t c = src_p[i];
        if (c.rgba.a > 5) {
          if (c.rgba.a > 0xfe) {
            dst_p[i] = to_pixel(c);
          } else {
            dst_p[i] = blend_pixel(dst_p[i], c);
          }
        }
      }
      src_p += img->w;
      dst_p += width;
    }
  } else {
    xy_t sx = src->x;
    xy_t sy = src->y;
    xy_t sw = src->w;
    xy_t sh = src->h;
    for (j = 0; j < dh; j++) {
      const color_t* src_p = data + img->w * (sy + (j * sh / dh)) + sx;
      for (i = 0; i < dw; i++) {
        color_t c = src_p[i * sw / dw];
        if (c.rgba.a > 5) {
          if (c.rgba.a > 0xfe) {
            dst_p[i] = to_pixel(c);
          } else {
            dst_p[i] = blend_pixel(dst_p[i], c);
          }
        }
      }
      dst_p += width;
    }
  }

  return RET_OK;
}

static vgcanvas_t* lcd_mem_get_vgcanvas(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  if (mem->vgcanvas == NULL) {
    mem->vgcanvas = vgcanvas_create(lcd->w, lcd->h, (uint32_t*)(mem->pixels));
  }

  return mem->vgcanvas;
}

static ret_t snapshot_destroy(bitmap_t* img) {
  TKMEM_FREE((void*)(img->data));

  return RET_OK;
}

static ret_t lcd_mem_take_snapshot(lcd_t* lcd, bitmap_t* img) {
  uint32_t size = 0;
  void* data = NULL;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  memset(img, 0x00, sizeof(bitmap_t));

  img->w = lcd->w;
  img->h = lcd->h;
  img->format = LCD_FORMAT;
  if (img->format == BITMAP_FMT_RGBA) {
    uint32_t i = 0;
    uint32_t* s = NULL;
    uint32_t* d = NULL;
    uint32_t nr = img->w * img->h;
    size = img->w * img->h * 4;
    data = TKMEM_ALLOC(size);

    return_value_if_fail(data != NULL, RET_FAIL);

    s = (uint32_t*)(mem->pixels);
    d = (uint32_t*)data;

    for (i = 0; i < nr; i++, s++, d++) {
      uint8_t* ps = (uint8_t*)s;
      uint8_t* pd = (uint8_t*)d;
      pd[0] = ps[3];
      pd[1] = ps[2];
      pd[2] = ps[1];
      pd[3] = ps[0];
    }
  } else {
    return_value_if_fail(size > 0, RET_FAIL);
  }

  img->destroy = snapshot_destroy;
  img->data = (uint8_t*)data;

  return RET_OK;
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) { return RET_OK; }

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  TKMEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  system_info_t* info = system_info();

  lcd_mem_t* lcd = TKMEM_ZALLOC(lcd_mem_t);
  lcd_t* base = &(lcd->base);

  if (alloc) {
    lcd->pixels = (uint8_t*)TKMEM_ALLOC(w * h * sizeof(pixel_t));
    return_value_if_fail(lcd->pixels != NULL, NULL);
  }

  base->begin_frame = lcd_mem_begin_frame;
  base->draw_vline = lcd_mem_draw_vline;
  base->draw_hline = lcd_mem_draw_hline;
  base->fill_rect = lcd_mem_fill_rect;
  base->draw_image = lcd_mem_draw_image;
  base->draw_glyph = lcd_mem_draw_glyph;
  base->draw_points = lcd_mem_draw_points;
  base->get_point_color = lcd_mem_get_point_color;
  base->get_vgcanvas = lcd_mem_get_vgcanvas;
  base->take_snapshot = lcd_mem_take_snapshot;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = lcd_mem_destroy;

  base->w = w;
  base->h = h;
  base->ratio = 1;
  base->type = LCD_FRAMEBUFFER;

  info->lcd_w = base->w;
  info->lcd_h = base->h;
  info->lcd_type = base->type;
  info->device_pixel_ratio = 1;

  return base;
}
