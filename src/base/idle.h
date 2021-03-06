﻿/**
 * File:   idle.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  idle manager
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IDEL_H
#define TK_IDEL_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct idle_info_t;
typedef struct idle_info_t idle_info_t;

typedef ret_t (*idle_func_t)(const idle_info_t* idle);

typedef struct idle_info_t {
  idle_func_t on_idle;
  void* ctx;
  uint32_t id;
} idle_info_t;

/**
 * @class idle_t
 * @scriptable
 * @fake
 * idle函数在paint之后执行。
 */

/**
 * @method idle_add
 * 增加一个idle。
 * @static
 * @scriptable custom
 * @param {idle_func_t} on_idle idle回调函数。
 * @param {void*} ctx idle回调函数的上下文。
 *
 * @return {uint32_t} 返回idle的ID，0表示失败。
 */
uint32_t idle_add(idle_func_t on_idle, void* ctx);

/**
 * @method idle_remove
 * 删除指定的idle。
 * @static
 * @scriptable custom
 * @param {uint32_t} idle_id idleID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_remove(uint32_t idle_id);

/**
 * @method idle_find
 * 查找指定ID的idle。
 * @private
 *
 * @return {idle_info_t*} 返回idle的信息。
 */
const idle_info_t* idle_find(uint32_t idle_id);

/**
 * @method idle_dispatch
 * 调用全部idle的函数。
 * @private
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_dispatch(void);

/**
 * @method idle_count
 * 返回idle的个数。
 * @static
 *
 * @return {uint32_t} 返回idle的个数。
 */
uint32_t idle_count(void);

END_C_DECLS

#endif /*TK_IDEL_H*/
