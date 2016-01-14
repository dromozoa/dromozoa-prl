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
#include "handle_server.hpp"
#include "result.hpp"

namespace dromozoa {
  using bind::function;

  namespace {
    int impl_connect_to_vm(lua_State* L) {
      return push_result(L,
          PrlDevDisplay_ConnectToVm(
              get_handle(L, 1),
              opt_enum(L, 2, PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION)));
    }

    int impl_disconnect_from_vm(lua_State* L) {
      return push_result(L, PrlDevDisplay_DisconnectFromVm(get_handle(L, 1)));
    }

    int impl_get_config(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlVm_GetConfig(get_handle(L, 1), &handle);
      return push_result(L, result, handle);
    }

    int impl_send_key_event_ex(lua_State* L) {
      return push_result(L,
          PrlDevKeyboard_SendKeyEventEx(
              get_handle(L, 1),
              check_enum<PRL_KEY>(L, 2),
              check_enum<PRL_KEY_EVENT>(L, 3)));
    }

    int impl_send_key_pressed_and_released(lua_State* L) {
      return push_result(L,
          PrlDevKeyboard_SendKeyPressedAndReleased(
              get_handle(L, 1),
              check_enum<PRL_KEY>(L, 2)));
    }
  }

  int open_handle_virtual_machine(lua_State* L) {
    lua_newtable(L);

    if (PHT_VM_CONFIGURATION == PHT_VIRTUAL_MACHINE) {
      luaL_getmetatable(L, "dromozoa.prl.vm_configuration");
    } else {
      luaL_getmetatable(L, "dromozoa.prl.handle");
    }
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, "dromozoa.prl.virtual_machine");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    function<gc_handle>::set_field(L, "__gc");
    lua_pop(L, 1);

    return 1;
  }
}
