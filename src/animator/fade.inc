
static ret_t window_animator_open_fade_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = wa->easing(wa->time_percent);
  } else {
    wa->percent = 1 - wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_open_fade_draw_prev(window_animator_t* wa) {
  rect_t src;
  rect_t dst;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->prev_win;

  rect_init(src, win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);
  rect_init(dst, win->x, win->y, win->w, win->h);

  lcd_draw_image(c->lcd, &(wa->prev_img), &src, &dst);

  return RET_OK;
}

static ret_t window_animator_open_fade_draw_curr(window_animator_t* wa) {
  rect_t src;
  rect_t dst;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  widget_t* win = wa->curr_win;
  float_t alpha = wa->percent;
  rect_init(dst, win->x, win->y, win->w, win->h);
  rect_init(src, win->x * ratio, win->y * ratio, win->w * ratio, win->h * ratio);

#ifdef WITH_NANOVG
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  vgcanvas_save(vg);
  vgcanvas_set_global_alpha(vg, alpha);
  vgcanvas_draw_image(vg, &(wa->curr_img), src.x, src.y, src.w, src.h, dst.x, dst.y, dst.w, dst.h);
  vgcanvas_restore(vg);
#else
  lcd_set_global_alpha(c->lcd, alpha * 0xff);
  lcd_draw_image(c->lcd, &(wa->curr_img), &src, &dst);
#endif

  return RET_OK;
}

static window_animator_t* window_animator_create_fade(bool_t open) {
  window_animator_t* wa = TKMEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_fade_update_percent;
  wa->draw_prev_window = window_animator_open_fade_draw_prev;
  wa->draw_curr_window = window_animator_open_fade_draw_curr;

  return wa;
}
