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
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        lua_pushinteger(L, count);
      }
    }

    void impl_get_param_by_index(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_UINT32 i = luaL_checkinteger(L, 2) - 1;
      PRL_RESULT result = PrlResult_GetParamByIndex(check_handle(L, 1), i, &handle);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        new_handle(L, handle);
      }
    }
  }

  void initialize_result(lua_State* L) {
    lua_newtable(L);
    {
      luaL_getmetatable(L, "dromozoa.prl.handle");
      luaX_get_field(L, -1, "__index");
      luaX_set_metafield(L, -3, "__index");
      luaL_newmetatable(L, "dromozoa.prl.result");
      lua_pushvalue(L, -3);
      luaX_set_field(L, -2, "__index");
      luaX_get_field(L, -2, "__gc");
      luaX_set_field(L, -2, "__gc");
      lua_pop(L, 2);

      luaX_set_field(L, -1, "get_param_by_index", impl_get_param_by_index);
      luaX_set_field(L, -1, "get_params_count", impl_get_params_count);

    }
    luaX_set_field(L, -2, "result");
  }
}
