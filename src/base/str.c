/**
 * File:   str.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  string
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
 * 2018-04-30 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/mem.h"
#include "base/utf8.h"
#include "base/str.h"
#include "base/utils.h"

static ret_t str_extend(str_t* str, uint16_t capacity) {
  if (capacity < str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    char* s = TKMEM_REALLOC(char, str->str, capacity + 1);
    return_value_if_fail(s != NULL, RET_FAIL);

    s[capacity] = '\0';
    str->str = s;
    str->capacity = capacity;
  }

  return RET_OK;
}

str_t* str_init(str_t* str, uint16_t capacity) {
  return_value_if_fail(str != NULL, NULL);

  memset(str, 0x00, sizeof(str_t));

  return str_extend(str, capacity) == RET_OK ? str : NULL;
}

ret_t str_set(str_t* str, const char* text) { return str_set_with_len(str, text, 0xffff); }

ret_t str_set_with_len(str_t* str, const char* text, uint16_t len) {
  uint16_t size = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  size = strlen(text);
  if (len <= size) {
    size = len;
  }
  return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_BAD_PARAMS);

  strncpy(str->str, text, size);
  str->str[size] = '\0';
  str->size = size;

  return RET_OK;
}

ret_t str_append(str_t* str, const char* text) {
  uint16_t size = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  size = strlen(text);
  return_value_if_fail(str_extend(str, str->size + size + 1) == RET_OK, RET_BAD_PARAMS);

  strcpy(str->str + str->size, text);
  str->size += size;

  return RET_OK;
}

bool_t str_eq(str_t* str, const char* text) {
  if ((str == NULL && text == NULL) || (str != NULL && str->str == NULL && text == NULL)) {
    return TRUE;
  }

  return_value_if_fail(str != NULL && text != NULL, FALSE);
  if (str->str[0] != text[0]) {
    return FALSE;
  }

  return strcmp(str->str, text) == 0;
}

ret_t str_from_int(str_t* str, int32_t v) {
  char buff[32];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, ftk_itoa(buff, sizeof(buff), v));
}

ret_t str_from_float(str_t* str, float v) {
  char buff[32];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, ftk_ftoa(buff, sizeof(buff), v));
}

ret_t str_from_value(str_t* str, const value_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_STRING) {
    return str_set(str, value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    /*TODO*/
    return RET_FAIL;
  } else if (v->type == VALUE_TYPE_FLOAT) {
    return str_from_float(str, value_float(v));
  } else {
    return str_from_int(str, value_int(v));
  }
}

ret_t str_to_int(str_t* str, int32_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  *v = ftk_atoi(str->str);

  return RET_OK;
}

ret_t str_to_float(str_t* str, float* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  *v = ftk_atof(str->str);

  return RET_OK;
}

ret_t str_reset(str_t* str) {
  return_value_if_fail(str != NULL, RET_OK);
  TKMEM_FREE(str->str);

  return RET_OK;
}

bool_t str_end_with(str_t* s, const char* str) {
  size_t len = 0;
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  len = strlen(str);
  if (len > s->size) {
    return FALSE;
  }

  return strncmp(s->str + s->size - len, str, len) == 0;
}

bool_t str_start_with(str_t* s, const char* str) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  return strncmp(s->str, str, strlen(str)) == 0;
}

ret_t str_trim_left(str_t* s, const char* str) {
  uint32_t i = 0;
  uint32_t k = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  if (!s->size) {
    return RET_OK;
  }

  if (str == NULL) {
    str = " ";
  }

  for (i = 0; i < n; i++) {
    char c = p[i];
    if (strchr(str, c) == NULL) {
      break;
    }
  }

  s->size = n - i;
  for (k = 0; i < n; i++, k++) {
    p[k] = p[i];
  }
  s->str[s->size] = '\0';

  return RET_OK;
}

ret_t str_trim_right(str_t* s, const char* str) {
  int32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  if (!s->size) {
    return RET_OK;
  }

  if (str == NULL) {
    str = " ";
  }

  for (i = n - 1; i >= 0; i--) {
    char c = p[i];
    if (strchr(str, c) == NULL) {
      break;
    }
  }

  p[i + 1] = '\0';
  s->size = i + 1;

  return RET_OK;
}

ret_t str_trim(str_t* s, const char* str) {
  if (!s->size) {
    return RET_OK;
  }

  str_trim_left(s, str);

  return str_trim_right(s, str);
}

static uint32_t str_count_sub_str(str_t* s, const char* str) {
  char* p = s->str;
  uint32_t count = 0;
  uint32_t size = strlen(str);

  do {
    p = strstr(p, str);
    if (p != NULL) {
      count++;
      p += size;
    }
  } while (p != NULL);

  return count;
}

ret_t str_replace(str_t* s, const char* str, const char* new_str) {
  uint32_t count = 0;

  return_value_if_fail(s != NULL && s->str != NULL && str != NULL && new_str != NULL,
                       RET_BAD_PARAMS);

  count = str_count_sub_str(s, str);

  if (count > 0) {
    char* p = s->str;
    char* src = s->str;
    uint32_t str_len = strlen(str);
    uint32_t new_str_len = strlen(new_str);
    uint32_t capacity = s->size + count * (strlen(new_str) - strlen(str)) + 1;

    char* temp_str = (char*)TKMEM_ALLOC(capacity);
    char* dst = temp_str;
    return_value_if_fail(temp_str != NULL, RET_OOM);
    do {
      uint32_t size = 0;
      p = strstr(src, str);
      if (p != NULL) {
        size = (uint32_t)(p - src);
      } else {
        size = (uint32_t)strlen(src);
      }
      memcpy(dst, src, size);
      src += size;
      dst += size;
      if (p != NULL) {
        if (new_str_len > 0) {
          memcpy(dst, new_str, new_str_len);
          dst += new_str_len;
        }
        src += str_len;
      }
      *dst = '\0';
    } while (p != NULL);

    TKMEM_FREE(s->str);
    s->str = temp_str;
    s->size = strlen(s->str);
    s->capacity = capacity;
  }

  return RET_OK;
}

ret_t str_to_lower(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  for (i = 0; i < n; i++) {
    p[i] = tolower(p[i]);
  }

  return RET_OK;
}

ret_t str_to_upper(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  for (i = 0; i < n; i++) {
    p[i] = toupper(p[i]);
  }

  return RET_OK;
}
