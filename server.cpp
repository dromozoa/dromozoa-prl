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
    void impl_create(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlSrv_Create(&handle);
      if (PRL_SUCCEEDED(result)) {
        new_handle(L, handle);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_vm_list(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_GetVmList(check_handle(L, 1));
      if (handle == PRL_INVALID_HANDLE) {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        new_handle(L, handle);
      }
    }

    void impl_login_local(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_LoginLocal(
          check_handle(L, 1),
          lua_tostring(L, 2),
          luaX_opt_integer<PRL_UINT32>(L, 3, 0),
          luaX_opt_enum(L, 4, PSL_NORMAL_SECURITY));
      if (handle == PRL_INVALID_HANDLE) {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        new_handle(L, handle);
      }
    }

    void impl_logoff(lua_State* L) {
      PRL_HANDLE handle = PrlSrv_Logoff(check_handle(L, 1));
      if (handle == PRL_INVALID_HANDLE) {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        new_handle(L, handle);
      }
    }
  }

  void initialize_server(lua_State* L) {
    lua_newtable(L);
    {
      inherit_handle(L, "dromozoa.prl.server");

      luaX_set_field(L, -1, "create", impl_create);
      luaX_set_field(L, -1, "get_vm_list", impl_get_vm_list);
      luaX_set_field(L, -1, "login_local", impl_login_local);
      luaX_set_field(L, -1, "logoff", impl_logoff);
    }
    luaX_set_field(L, -2, "server");
  }
}
