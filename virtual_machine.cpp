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
    void impl_connect_to_vm(lua_State* L) {
      PRL_HANDLE handle = PrlDevDisplay_ConnectToVm(
          check_handle(L, 1),
          luaX_opt_enum(L, 2, PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION));
      if (handle == PRL_INVALID_HANDLE) {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      } else {
        new_handle(L, handle);
      }
    }

    void impl_disconnect_from_vm(lua_State* L) {
      PRL_RESULT result = PrlDevDisplay_DisconnectFromVm(check_handle(L, 1));
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_config(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlVm_GetConfig(check_handle(L, 1), &handle);
      if (PRL_SUCCEEDED(result)) {
        new_handle(L, handle);
      } else {
        push_error(L, result);
      }
    }

    void impl_send_key_event_ex(lua_State* L) {
      PRL_RESULT result = PrlDevKeyboard_SendKeyEventEx(
          check_handle(L, 1),
          luaX_check_enum<PRL_KEY>(L, 2),
          luaX_check_enum<PRL_KEY_EVENT>(L, 3));
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_send_key_pressed_and_released(lua_State* L) {
      PRL_RESULT result = PrlDevKeyboard_SendKeyPressedAndReleased(
          check_handle(L, 1),
          luaX_check_enum<PRL_KEY>(L, 2));
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }
  }

  void initialize_virtual_machine(lua_State* L) {
    lua_newtable(L);
    {
      if (PHT_VIRTUAL_MACHINE == PHT_VM_CONFIGURATION) {
        inherit_handle(L, "dromozoa.prl.virtual_machine", "dromozoa.prl.vm_configuration");
      } else {
        inherit_handle(L, "dromozoa.prl.virtual_machine");
      }

      luaX_set_field(L, -1, "connect_to_vm", impl_connect_to_vm);
      luaX_set_field(L, -1, "disconnect_from_vm", impl_disconnect_from_vm);
      luaX_set_field(L, -1, "get_config", impl_get_config);
      luaX_set_field(L, -1, "send_key_event_ex", impl_send_key_event_ex);
      luaX_set_field(L, -1, "send_key_pressed_and_released", impl_send_key_pressed_and_released);
    }
    luaX_set_field(L, -2, "virtual_machine");
  }
}
