// Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-prl.
//
// dromozoa-prl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-prl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <iostream>

#include "dromozoa/bind.hpp"

#include "error.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace dromozoa {
  using bind::get_log_level;

  lua_Integer get_handle_address(PRL_HANDLE handle) {
    return reinterpret_cast<lua_Integer>(handle);
  }

  int new_handle(lua_State* L, PRL_HANDLE handle) {
    PRL_HANDLE_TYPE type = PHT_ERROR;
    PrlHandle_GetType(handle, &type);

    const char* name = "dromozoa.prl.handle";
    switch (type) {
      case PHT_JOB:
        name = "dromozoa.prl.job";
        break;
      case PHT_RESULT:
        name = "dromozoa.prl.result";
        break;
      case PHT_SERVER:
        name = "dromozoa.prl.server";
        break;
      case PHT_VIRTUAL_MACHINE:
        name = "dromozoa.prl.virtual_machine";
        break;
      default:
        break;
    }

    *static_cast<PRL_HANDLE*>(lua_newuserdata(L, sizeof(PRL_HANDLE))) = handle;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
    if (get_log_level() > 2) {
      std::cerr << "[dromozoa-prl] new handle " << get_handle_address(handle) << " (" << handle_type_to_string(type) << ")" << std::endl;
    }

    return 1;
  }

  PRL_HANDLE get_handle(lua_State* L, int n) {
    if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, n))) {
      return *data;
    } else {
      return PRL_INVALID_HANDLE;
    }
  }

  PRL_RESULT free_handle(PRL_HANDLE handle) {
    if (PrlHandle_Free) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      PrlHandle_GetType(handle, &type);
      PRL_RESULT result = PrlHandle_Free(handle);
      if (PRL_SUCCEEDED(result)) {
        if (get_log_level() > 2) {
          std::cerr << "[dromozoa-prl] detach handle " << get_handle_address(handle) << " (" << handle_type_to_string(type) << ")" << std::endl;
        }
      }
      return result;
    } else {
      return PRL_ERR_UNINITIALIZED;
    }
  }

  int gc_handle(lua_State* L) {
    if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
      PRL_HANDLE handle = *data;
      *data = PRL_INVALID_HANDLE;
      if (handle != PRL_INVALID_HANDLE) {
        if (get_log_level() > 1) {
          std::cerr << "[dromozoa-prl] handle " << get_handle_address(handle) << " detected" << std::endl;
        }
        PRL_RESULT result = free_handle(handle);
        if (PRL_FAILED(result)) {
          if (get_log_level() > 0) {
            std::cerr << "[dromozoa-prl] cannot free handle " << get_handle_address(handle) << ": ";
            print_error(std::cerr, result);
            std::cerr << std::endl;
          }
        }
      }
    }
    return 0;
  }

  namespace {
    int impl_free(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        PRL_RESULT result = free_handle(*data);
        if (PRL_SUCCEEDED(result)) {
          *data = PRL_INVALID_HANDLE;
        }
        return push_result(L, result);
      } else {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      }
    }
  }

  int open_handle_(lua_State* L) {
    lua_newtable(L);

    return 1;
  }
}
