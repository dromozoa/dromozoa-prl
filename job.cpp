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
    void impl_wait(lua_State* L) {
      PRL_RESULT result = PrlJob_Wait(
          check_handle(L, 1),
          luaX_opt_integer<PRL_UINT32>(L, 2, std::numeric_limits<PRL_UINT32>::max()));
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        luaX_push_success(L);
      }
    }

    void impl_get_ret_code(lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(check_handle(L, 1), &code);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        push_error_string(L, code);
        lua_pushinteger(L, code);
      }
    }

    void impl_check_ret_code(lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(check_handle(L, 1), &code);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      }
      if (PRL_FAILED(code)) {
        push_error(L, code);
      } else {
        luaX_push_success(L);
      }
    }

    void impl_get_result(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlJob_GetResult(check_handle(L, 1), &handle);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        new_handle(L, handle);
      }
    }

    void impl_get_result_and_free(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        PRL_HANDLE handle = PRL_INVALID_HANDLE;
        PRL_RESULT result = PrlJob_GetResult(*data, &handle);
        if (PRL_FAILED(result)) {
          push_error(L, result);
        }
        result = free_handle(*data);
        if (PRL_SUCCEEDED(result)) {
          *data = PRL_INVALID_HANDLE;
        }
        new_handle(L, handle);
      } else {
        push_error(L, PRL_ERR_INVALID_HANDLE);
      }
    }
  }

  void initialize_job(lua_State* L) {
    lua_newtable(L);
    {
      luaX_set_field(L, -1, "wait", impl_wait);
      luaX_set_field(L, -1, "get_ret_code", impl_get_ret_code);
      luaX_set_field(L, -1, "check_ret_code", impl_check_ret_code);
      luaX_set_field(L, -1, "get_result", impl_get_result);
      luaX_set_field(L, -1, "get_result_and_free", impl_get_result_and_free);

      luaL_getmetatable(L, "dromozoa.prl.handle");
      lua_setmetatable(L, -2);

      luaL_newmetatable(L, "dromozoa.prl.job");
      lua_pushvalue(L, -2);
      lua_setfield(L, -2, "__index");
      initialize_handle_gc(L);
      lua_pop(L, 1);
    }
    luaX_set_field(L, -2, "job");
  }
}
