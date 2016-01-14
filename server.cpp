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

#include "dromozoa/bind.hpp"

#include "enum.hpp"
#include "error.hpp"
#include "handle.hpp"
#include "server.hpp"

namespace dromozoa {
  using bind::function;

  namespace {
    int impl_create(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlSrv_Create(&handle);
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return new_handle(L, handle);
      }
    }

    int impl_get_vm_list(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_GetVmList(get_handle(L, 1));
      if (handle == PRL_INVALID_HANDLE) {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        return new_handle(L, handle);
      }
    }

    int impl_login_local(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_LoginLocal(
          get_handle(L, 1),
          lua_tostring(L, 2),
          luaL_optinteger(L, 3, 0),
          opt_enum(L, 4, PSL_NORMAL_SECURITY));
      if (handle == PRL_INVALID_HANDLE) {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        return new_handle(L, handle);
      }
    }

    int impl_logoff(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_Logoff(get_handle(L, 1));
      if (handle == PRL_INVALID_HANDLE) {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        return new_handle(L, handle);
      }
    }
  }

  int open_server(lua_State* L) {
    lua_newtable(L);
    function<impl_create>::set_field(L, "create");
    function<impl_get_vm_list>::set_field(L, "get_vm_list");
    function<impl_login_local>::set_field(L, "login_local");
    function<impl_logoff>::set_field(L, "logoff");

    luaL_getmetatable(L, "dromozoa.prl.handle");
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, "dromozoa.prl.server");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    initialize_handle_gc(L);
    lua_pop(L, 1);

    return 1;
  }
}
