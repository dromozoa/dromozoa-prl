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

#include "common.hpp"

namespace dromozoa {
  namespace {
    lua_Integer get_handle_address(PRL_HANDLE handle) {
      return reinterpret_cast<lua_Integer>(handle);
    }
  }

  void new_handle(lua_State* L, PRL_HANDLE handle) {
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

    luaX_new<PRL_HANDLE>(L, handle);
    luaX_set_metatable(L, name);
  }

  PRL_RESULT free_handle(PRL_HANDLE handle) {
    if (PrlHandle_Free) {
      PRL_RESULT result = PrlHandle_Free(handle);
      if (PRL_SUCCEEDED(result)) {
      }
      return result;
    } else {
      return PRL_ERR_UNINITIALIZED;
    }
  }

  PRL_HANDLE check_handle(lua_State* L, int arg) {
    return *luaX_check_udata<PRL_HANDLE>(L, arg,
        "dromozoa.prl.job",
        "dromozoa.prl.result",
        "dromozoa.prl.server",
        "dromozoa.prl.virtual_machine",
        "dromozoa.prl.vm_configuration",
        "dromozoa.prl.handle");
  }

  namespace {
    void impl_free(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        PRL_RESULT result = free_handle(*data);
        if (PRL_SUCCEEDED(result)) {
          *data = PRL_INVALID_HANDLE;
        }
        luaX_push_success(L);
      } else {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      }
    }

    void impl_gc(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        *data = PRL_INVALID_HANDLE;
      }
    }

    void impl_get_type(lua_State* L) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      PRL_RESULT result = PrlHandle_GetType(check_handle(L, 1), &type);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        lua_pushstring(L, handle_type_to_string(type));
        lua_pushinteger(L, type);
      }
    }

    void impl_get_address(lua_State* L) {
      lua_pushinteger(L, get_handle_address(check_handle(L, 1)));
    }
  }

  void initialize_handle_gc(lua_State* L) {
    luaX_set_field(L, -1, "__gc", impl_gc);
  }

  void inherit_handle(lua_State* L, const char* name) {
    inherit_handle(L, name, "dromozoa.prl.handle");
  }

  void inherit_handle(lua_State* L, const char* name, const char* super) {
    luaL_getmetatable(L, super);
    luaX_get_field(L, -1, "__index");
    luaX_set_metafield(L, -3, "__index");
    luaL_newmetatable(L, name);
    lua_pushvalue(L, -3);
    luaX_set_field(L, -2, "__index");
    luaX_get_field(L, -2, "__gc");
    luaX_set_field(L, -2, "__gc");
    lua_pop(L, 2);
  }

  void initialize_handle(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.prl.handle");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_field(L, -1, "free", impl_free);
      luaX_set_field(L, -1, "get_type", impl_get_type);
      luaX_set_field(L, -1, "get_address", impl_get_address);
    }
    luaX_set_field(L, -2, "handle");
  }
}
