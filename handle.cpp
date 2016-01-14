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

namespace dromozoa {
  using bind::function;
  using bind::get_log_level;
  using bind::push_success;

  namespace {
    lua_Integer get_handle_address(PRL_HANDLE handle) {
      return reinterpret_cast<lua_Integer>(handle);
    }
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
        if (type == PHT_VM_CONFIGURATION) {
          name = "dromozoa.prl.vm_configuration";
        }
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
      PRL_RESULT result = PrlHandle_Free(handle);
      if (PRL_SUCCEEDED(result)) {
        if (get_log_level() > 2) {
          std::cerr << "[dromozoa-prl] free handle " << get_handle_address(handle) << std::endl;
        }
      }
      return result;
    } else {
      return PRL_ERR_UNINITIALIZED;
    }
  }

  namespace {
    int impl_free(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        PRL_RESULT result = free_handle(*data);
        if (PRL_SUCCEEDED(result)) {
          *data = PRL_INVALID_HANDLE;
        }
        return push_success(L);
      } else {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      }
    }

    int impl_gc(lua_State* L) {
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

    int impl_get_type(lua_State* L) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      PRL_RESULT result = PrlHandle_GetType(get_handle(L, 1), &type);
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        lua_pushstring(L, handle_type_to_string(type));
        lua_pushinteger(L, type);
        return 2;
      }
    }

    int impl_get_address(lua_State* L) {
      lua_pushinteger(L, get_handle_address(get_handle(L, 1)));
      return 1;
    }
  }

  void initialize_handle_gc(lua_State* L) {
    function<impl_gc>::set_field(L, "__gc");
  }

  int open_handle(lua_State* L) {
    lua_newtable(L);
    function<impl_free>::set_field(L, "free");
    function<impl_get_type>::set_field(L, "get_type");
    function<impl_get_address>::set_field(L, "get_address");

    luaL_newmetatable(L, "dromozoa.prl.handle");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    initialize_handle_gc(L);
    lua_pop(L, 1);

    return 1;
  }
}
