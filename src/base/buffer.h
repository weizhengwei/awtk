﻿/**
 * File:   buffer.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  buffer
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUFFER_H
#define TK_BUFFER_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class wbuffer_t
 * write buffer，用于数据打包。
 */
typedef struct _wbuffer_t {
  /**
   * @property {uint8_t*} data
   * @readonly
   * 缓存区。
   */
  uint8_t* data;
  /**
   * @property {uint32_t} cursor
   * @readonly
   * 当前写入位置。
   */
  uint32_t cursor;
  /**
   * @property {uint32_t} capacity
   * @readonly
   * 缓存区最大容量。
   */
  uint32_t capacity;
} wbuffer_t;

/**
 * @method wbuffer_init
 * 初始wbuffer对象。
 * @constructor
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint8_t*} data 缓冲区。
 * @param {uint16_t} capacity 缓冲区长度。
 *
 * @return {wbuffer_t*} wbuffer对象本身。
 */
wbuffer_t* wbuffer_init(wbuffer_t* wbuffer, uint8_t* data, uint32_t capacity);

/**
 * @method wbuffer_skip
 * 跳过指定的长度。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {int32_t} offset 跳过的偏移量，正数往前负数往回跳。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_skip(wbuffer_t* wbuffer, int32_t offset);

/**
 * @method wbuffer_write_uint8
 * 写入uint8数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint8_t} value 写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint8(wbuffer_t* wbuffer, uint8_t value);

/**
 * @method wbuffer_write_uint16
 * 写入uint16数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint16_t} value 写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint16(wbuffer_t* wbuffer, uint16_t value);

/**
 * @method wbuffer_write_uint32
 * 写入uint32数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {uint32_t} value 写入的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_uint32(wbuffer_t* wbuffer, uint32_t value);

/**
 * @method wbuffer_write_binary
 * 写入指定长度的二进制数据。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {void*} data 写入的数据。
 * @param {uint32_t} size 写入的数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_binary(wbuffer_t* wbuffer, const void* data, uint32_t size);

/**
 * @method wbuffer_write_string
 * 写入字符串。
 * @param {wbuffer_t*} wbuffer wbuffer对象。
 * @param {char*} data 写入的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wbuffer_write_string(wbuffer_t* wbuffer, const char* data);

/**
 * @class rbuffer_t
 * read buffer，用于数据解包。
 */
typedef struct _rbuffer_t {
  /**
   * @property {uint8_t*} data
   * @readonly
   * 缓存区。
   */
  const uint8_t* data;
  /**
   * @property {uint32_t} cursor
   * @readonly
   * 当前读取位置。
   */
  uint32_t cursor;
  /**
   * @property {uint32_t} capacity
   * @readonly
   * 缓存区最大容量。
   */
  uint32_t capacity;
} rbuffer_t;

/**
 * @method rbuffer_init
 * 初始rbuffer对象。
 * @constructor
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} data 缓冲区。
 * @param {uint16_t} capacity 缓冲区长度。
 *
 * @return {rbuffer_t*} rbuffer对象本身。
 */
rbuffer_t* rbuffer_init(rbuffer_t* rbuffer, const uint8_t* data, uint32_t capacity);

/**
 * @method rbuffer_has_more
 * 判断是否还有数据可读。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 *
 * @return {bool_t} 返回TRUE表示还有数据可读，否则表示无数据可读。
 */
bool_t rbuffer_has_more(rbuffer_t* rbuffer);

/**
 * @method rbuffer_skip
 * 跳过指定的长度。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {int32_t} offset 跳过的偏移量，正数往前负数往回跳。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_skip(rbuffer_t* rbuffer, int32_t offset);

/**
 * @method rbuffer_read_uint8
 * 读取uint8数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint8(rbuffer_t* rbuffer, uint8_t* value);

/**
 * @method rbuffer_read_uint16
 * 读取uint16数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint16_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint16(rbuffer_t* rbuffer, uint16_t* value);

/**
 * @method rbuffer_read_uint32
 * 读取uint32数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint32_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_uint32(rbuffer_t* rbuffer, uint32_t* value);

/**
 * @method rbuffer_read_binary
 * 读取指定长度的二进制数据。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {void*} data 读取的数据。
 * @param {uint32_t} size 读取的数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_binary(rbuffer_t* rbuffer, void* data, uint32_t size);

/**
 * @method rbuffer_read_string
 * 读取字符串。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {char**} str 用于返回字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_read_string(rbuffer_t* rbuffer, const char** str);

/**
 * @method rbuffer_peek_uint8
 * 读取uint8数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint8_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint8(rbuffer_t* rbuffer, uint8_t* value);

/**
 * @method rbuffer_peek_uint16
 * 读取uint16数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint16_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint16(rbuffer_t* rbuffer, uint16_t* value);

/**
 * @method rbuffer_peek_uint32
 * 读取uint32数据，但不改变cursor的位置。
 * @param {rbuffer_t*} rbuffer rbuffer对象。
 * @param {uint32_t*} value 读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rbuffer_peek_uint32(rbuffer_t* rbuffer, uint32_t* value);

END_C_DECLS

#endif /*TK_BUFFER_H*/
