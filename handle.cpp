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
    void impl_gc(lua_State* L) {
      check_handle_reference(L, 1)->~handle_reference();
    }

    void impl_free(lua_State* L) {
      PRL_RESULT result = check_handle_reference(L, 1)->free();
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_address(lua_State* L) {
      luaX_push(L, check_handle_reference(L, 1)->get_address());
    }

    void impl_get_type(lua_State* L) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      PRL_RESULT result = PrlHandle_GetType(check_handle(L, 1), &type);
      if (PRL_SUCCEEDED(result)) {
        luaX_push(L, handle_type_to_string(type));
        luaX_push<lua_Integer>(L, type);
      } else {
        push_error(L, result);
      }
    }
  }

  handle_reference::handle_reference(PRL_HANDLE handle) : handle_(handle) {}

  handle_reference::~handle_reference() {
    if (handle_ != PRL_INVALID_HANDLE) {
      PRL_RESULT result = free();
      if (PRL_FAILED(result)) {
        DROMOZOA_UNEXPECTED(result_to_string(result));
      }
    }
  }

  PRL_RESULT handle_reference::free() {
    PRL_HANDLE handle = handle_;
    handle_ = PRL_INVALID_HANDLE;
    if (PrlHandle_Free) {
      return PrlHandle_Free(handle);
    } else {
      return PRL_ERR_UNINITIALIZED;
    }
  }

  PRL_HANDLE handle_reference::get() const {
    return handle_;
  }

  intptr_t handle_reference::get_address() const {
    return reinterpret_cast<intptr_t>(handle_);
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

    luaX_new<handle_reference>(L, handle);
    luaX_set_metatable(L, name);
  }

  handle_reference* check_handle_reference(lua_State* L, int arg) {
    return luaX_check_udata<handle_reference>(L, arg,
        "dromozoa.prl.job",
        "dromozoa.prl.result",
        "dromozoa.prl.server",
        "dromozoa.prl.virtual_machine",
        "dromozoa.prl.vm_configuration",
        "dromozoa.prl.handle");
  }

  PRL_HANDLE check_handle(lua_State* L, int arg) {
    return check_handle_reference(L, arg)->get();
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

  void initialize_job(lua_State* L);
  void initialize_result(lua_State* L);
  void initialize_server(lua_State* L);
  void initialize_vm_configuration(lua_State* L);

  void initialize_handle(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.prl.handle");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_field(L, -1, "free", impl_free);
      luaX_set_field(L, -1, "get_address", impl_get_address);
      luaX_set_field(L, -1, "get_type", impl_get_type);
    }
    luaX_set_field(L, -2, "handle");

    initialize_job(L);
    initialize_result(L);
    initialize_server(L);
    initialize_vm_configuration(L);
  }
}
