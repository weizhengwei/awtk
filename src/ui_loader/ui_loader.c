/**
 * File:   ui_loader.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  ui_loader interface
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ui_loader/ui_loader.h"
#include "base/resource_manager.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

ret_t ui_loader_load(ui_loader_t* loader, const uint8_t* data, uint32_t size, ui_builder_t* b) {
  return_value_if_fail(loader != NULL && loader->load != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL && b != NULL, RET_BAD_PARAMS);

  return loader->load(loader, data, size, b);
}
