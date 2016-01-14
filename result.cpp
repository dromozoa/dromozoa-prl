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

#include "error.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace dromozoa {
  using bind::function;

  namespace {
    int impl_get_params_count(lua_State* L) {
      PRL_UINT32 count = 0;
      PRL_RESULT result = PrlResult_GetParamsCount(get_handle(L, 1), &count);
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        lua_pushinteger(L, count);
        return 1;
      }
    }

    int impl_get_param_by_index(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_UINT32 i = luaL_checkinteger(L, 2) - 1;
      PRL_RESULT result = PrlResult_GetParamByIndex(get_handle(L, 1), i, &handle);
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return new_handle(L, handle);
      }
    }
  }

  int open_result(lua_State* L) {
    lua_newtable(L);
    function<impl_get_param_by_index>::set_field(L, "get_param_by_index");
    function<impl_get_params_count>::set_field(L, "get_params_count");

    luaL_getmetatable(L, "dromozoa.prl.handle");
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, "dromozoa.prl.result");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    initialize_handle_gc(L);
    lua_pop(L, 1);

    return 1;
  }
}
