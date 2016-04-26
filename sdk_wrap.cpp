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
    void impl_is_loaded(lua_State* L) {
      luaX_push<bool>(L, SdkWrap_IsLoaded());
    }

    void impl_load(lua_State* L) {
      PRL_RESULT result = SdkWrap_Load(luaL_checkstring(L, 1), lua_toboolean(L, 2));
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_load_lib_from_std_paths(lua_State* L) {
      PRL_RESULT result = SdkWrap_LoadLibFromStdPaths(lua_toboolean(L, 1));
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_unload(lua_State* L) {
      PRL_RESULT result = SdkWrap_Unload();
      if (PRL_SUCCEEDED(result)) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }
  }

  void initialize_sdk_wrap(lua_State* L) {
    lua_newtable(L);
    {
      luaX_set_field(L, -1, "is_loaded", impl_is_loaded);
      luaX_set_field(L, -1, "load", impl_load);
      luaX_set_field(L, -1, "load_lib_from_std_paths", impl_load_lib_from_std_paths);
      luaX_set_field(L, -1, "unload", impl_unload);
    }
    luaX_set_field(L, -2, "sdk_wrap");
  }
}
