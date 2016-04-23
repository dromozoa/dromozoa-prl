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

#include <SdkWrap.h>

#include <dromozoa/bind.hpp>

#include "error.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace dromozoa {
  namespace {
    void impl_get_params_count(lua_State* L) {
      PRL_UINT32 count = 0;
      PRL_RESULT result = PrlResult_GetParamsCount(get_handle(L, 1), &count);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        lua_pushinteger(L, count);
      }
    }

    void impl_get_param_by_index(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_UINT32 i = luaL_checkinteger(L, 2) - 1;
      PRL_RESULT result = PrlResult_GetParamByIndex(get_handle(L, 1), i, &handle);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        new_handle(L, handle);
      }
    }
  }

  void open_result(lua_State* L) {
    lua_newtable(L);
    luaX_set_field(L, -1, "get_param_by_index", impl_get_param_by_index);
    luaX_set_field(L, -1, "get_params_count", impl_get_params_count);

    luaL_getmetatable(L, "dromozoa.prl.handle");
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, "dromozoa.prl.result");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    initialize_handle_gc(L);
    lua_pop(L, 1);
  }
}
