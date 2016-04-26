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
    void impl_get_params_count(lua_State* L) {
      PRL_UINT32 count = 0;
      PRL_RESULT result = PrlResult_GetParamsCount(check_handle(L, 1), &count);
      if (PRL_SUCCEEDED(result)) {
        luaX_push(L, count);
      } else {
        push_error(L, result);
      }
    }

    void impl_get_param_by_index(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_UINT32 i = luaX_check_integer<PRL_UINT32>(L, 2) - 1;
      PRL_RESULT result = PrlResult_GetParamByIndex(check_handle(L, 1), i, &handle);
      if (PRL_SUCCEEDED(result)) {
        new_handle(L, handle);
      } else {
        push_error(L, result);
      }
    }
  }

  void initialize_result(lua_State* L) {
    lua_newtable(L);
    {
      inherit_handle(L, "dromozoa.prl.result");
      luaX_set_field(L, -1, "get_param_by_index", impl_get_param_by_index);
      luaX_set_field(L, -1, "get_params_count", impl_get_params_count);

    }
    luaX_set_field(L, -2, "result");
  }
}
