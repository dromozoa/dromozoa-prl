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
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_ret_code(lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(check_handle(L, 1), &code);
      if (PRL_SUCCEEDED(result)) {
        luaX_push(L, result_to_string(code));
        luaX_push(L, code);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_result(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlJob_GetResult(check_handle(L, 1), &handle);
      if (PRL_SUCCEEDED(result)) {
        new_handle(L, handle);
      } else {
        push_error(L, result);
      }
    }
  }

  void initialize_job(lua_State* L) {
    lua_newtable(L);
    {
      inherit_handle(L, "dromozoa.prl.job");

      luaX_set_field(L, -1, "wait", impl_wait);
      luaX_set_field(L, -1, "get_ret_code", impl_get_ret_code);
      luaX_set_field(L, -1, "get_result", impl_get_result);
    }
    luaX_set_field(L, -2, "job");
  }
}
