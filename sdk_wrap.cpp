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
#include "sdk_wrap.hpp"

namespace dromozoa {
  using bind::function;
  using bind::push_success;

  namespace {
    int impl_is_loaded(lua_State* L) {
      lua_pushboolean(L, SdkWrap_IsLoaded());
      return 1;
    }

    int impl_load(lua_State* L) {
      PRL_RESULT result = SdkWrap_Load(luaL_checkstring(L, 1), lua_toboolean(L, 2));
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return push_success(L);
      }
    }

    int impl_load_lib_from_std_paths(lua_State* L) {
      PRL_RESULT result = SdkWrap_LoadLibFromStdPaths(lua_toboolean(L, 2));
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return push_success(L);
      }
    }

    int impl_unload(lua_State* L) {
      PRL_RESULT result = SdkWrap_Unload();
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return push_success(L);
      }
    }
  }

  int open_sdk_wrap(lua_State* L) {
    lua_newtable(L);
    function<impl_is_loaded>::set_field(L, "is_loaded");
    function<impl_load>::set_field(L, "load");
    function<impl_load_lib_from_std_paths>::set_field(L, "load_lib_from_std_paths");
    function<impl_unload>::set_field(L, "unload");
    return 1;
  }
}
