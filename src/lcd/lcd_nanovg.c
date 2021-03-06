﻿/**
 * File:   lcd.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lcd interface
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
 * 2018-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/lcd.h"
#include "base/vgcanvas.h"
#include "lcd/lcd_nanovg.h"
#include "lcd_vgcanvas.inc"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

lcd_t* lcd_nanovg_init(SDL_Window* sdl_window) {
  int w = 0;
  int h = 0;
  vgcanvas_t* vg = NULL;
  return_value_if_fail(sdl_window != NULL, NULL);

  SDL_GetWindowSize(sdl_window, &w, &h);
  vg = vgcanvas_create(w, h, sdl_window);
  return_value_if_fail(vg != NULL, NULL);

  return lcd_vgcanvas_init(w, h, vg);
}
