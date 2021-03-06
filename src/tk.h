﻿/**
 * File:   awtk.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  awtk
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
 * 2018-03-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MAIN_H
#define TK_MAIN_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @method tk_init
 * 初始化TK。
 * @global
 * @scriptable no
 * @param {wh_t} w LCD宽度。
 * @param {wh_t} h LCD高度。
 * @param {uint32_t*} heap 动态内存空间。
 * @param {uint32_t} size 动态内存空间的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_init(wh_t w, wh_t h, uint32_t* heap, uint32_t size);

/**
 * @method tk_run
 * 进入TK事件主循环。
 * @global
 * @scriptable no
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_run(void);

/**
 * @method tk_quit
 * 退出TK事件主循环。
 * @global
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_quit(void);

ret_t tk_init_resources(void);

END_C_DECLS

#endif /*TK_MAIN_H*/
